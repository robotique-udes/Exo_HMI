#include "ble_link.h"
#include <BLEDevice.h>
#include <BLEClient.h>
#include <BLEScan.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <cstring>
#include <cstdio>

namespace {
struct Command { int type; int index; char text[21]; };
struct Incoming { char text[64]; };
QueueHandle_t commands = nullptr;
QueueHandle_t incoming = nullptr;
portMUX_TYPE stateLock = portMUX_INITIALIZER_UNLOCKED;
ExoSnapshot shared = {};

BLEClient* client = nullptr;
BLERemoteCharacteristic* rx = nullptr;
BLEAdvertisedDevice* candidates[4] = {};
bool ready = false;

template<size_t N> void copyText(char (&dst)[N], const char* text) {
    snprintf(dst, N, "%s", text ? text : "");
}

void publish(ExoSnapshot& s) {
    ++s.revision;
    portENTER_CRITICAL(&stateLock);
    shared = s;
    portEXIT_CRITICAL(&stateLock);
}

void notified(BLERemoteCharacteristic*, uint8_t* bytes, size_t length, bool) {
    Incoming event = {};
    size_t n = length < sizeof(event.text)-1 ? length : sizeof(event.text)-1;
    memcpy(event.text, bytes, n);
    xQueueSend(incoming, &event, 0);
}

void worker(void*) {
    BLEDevice::init("BIOGENIUS-WATCH");
    client = BLEDevice::createClient();
    BLEScan* scanner = BLEDevice::getScan();
    scanner->setActiveScan(true);
    scanner->setInterval(100);
    scanner->setWindow(80);

    ExoSnapshot s = {};
    copyText(s.state, "Inconnu");
    copyText(s.message, "Pret. Appuyer sur Chercher.");
    publish(s);

    for (;;) {
        if (s.connected && !client->isConnected()) {
            s.connected = false;
            ready = false;
            rx = nullptr;
            copyText(s.peer, "");
            copyText(s.state, "Inconnu");
            copyText(s.message, "Liaison perdue.");
            publish(s);
        }

        Incoming event;
        while (xQueueReceive(incoming, &event, 0) == pdTRUE) {
            if (!s.connected) continue;
            copyText(s.message, event.text);
            if (strncmp(event.text, "STATE,", 6) == 0) {
                copyText(s.state, event.text + 6);
            }
            publish(s);
        }

        Command c;
        if (xQueueReceive(commands, &c, pdMS_TO_TICKS(30)) != pdTRUE) continue;

        if (c.type == 1) {
            if (s.connected) {
                copyText(s.message, "Deconnecter avant de rechercher.");
                publish(s);
                continue;
            }
            s.busy = true;
            s.count = 0;
            copyText(s.message, "Recherche BLE (5 secondes)...");
            for (int i=0; i<4; ++i) {
                delete candidates[i];
                candidates[i] = nullptr;
            }
            publish(s);

            // Bloquant uniquement dans cette tache, jamais dans la boucle UI.
            BLEScanResults results = scanner->start(5, false);

            Serial.printf(
                "\n[BLE] Recherche terminee : %d appareil(s) detecte(s)\n",
                results.getCount()
            );

            Serial.printf(
                "[BLE] Service recherche : %s\n",
                EXO_SERVICE_UUID
            );

            for (int i = 0; i < results.getCount(); ++i) {
                BLEAdvertisedDevice d = results.getDevice(i);

                String deviceName = d.haveName()
                    ? String(d.getName().c_str())
                    : String("");

                String normalizedName = deviceName;
                normalizedName.toLowerCase();

                const bool matchesService =
                    d.haveServiceUUID() &&
                    d.isAdvertisingService(BLEUUID(EXO_SERVICE_UUID));

                const bool matchesName =
                    normalizedName.indexOf("biogenius") >= 0;

                // Afficher TOUS les resultats dans le moniteur serie,
                // meme ceux qui ne seront pas affiches sur la montre.
                Serial.printf(
                    "[BLE] Nom: %s | Adresse: %s | RSSI: %d"
                    " | Service attendu: %s | Nom attendu: %s\n",
                    deviceName.length() ? deviceName.c_str() : "(sans nom)",
                    d.getAddress().toString().c_str(),
                    d.getRSSI(),
                    matchesService ? "OUI" : "NON",
                    matchesName ? "OUI" : "NON"
                );

                Serial.println(d.toString().c_str());

                // Accepter le service OU le nom de notre appareil.
                if (!matchesService && !matchesName) {
                    continue;
                }

                // Ne pas depasser les quatre emplacements de l'interface.
                if (s.count >= 4) {
                    continue;
                }

                bool duplicate = false;

                for (int j = 0; j < s.count; ++j) {
                    if (
                        candidates[j]->getAddress().toString() ==
                        d.getAddress().toString()
                    ) {
                        duplicate = true;
                        break;
                    }
                }

                if (duplicate) {
                    continue;
                }

                const int index = s.count++;

                candidates[index] = new BLEAdvertisedDevice(d);

                copyText(
                    s.devices[index].name,
                    deviceName.length()
                        ? deviceName.c_str()
                        : "ESP32 sans nom"
                );

                s.devices[index].rssi = d.getRSSI();
            }

            scanner->clearResults();

            s.busy = false;

            copyText(
                s.message,
                s.count
                    ? "Toucher un appareil pour connecter."
                    : "Aucun BioGenius trouve. Voir serie."
            );

            publish(s);
        }
        else if (c.type == 2) {
            if (s.connected || c.index<0 || c.index>=s.count) continue;
            s.busy = true;
            copyText(s.message, "Connexion en cours...");
            publish(s);
            ready = false;
            rx = nullptr;
            if (client->connect(candidates[c.index])) {
                BLERemoteService* service = client->getService(BLEUUID(EXO_SERVICE_UUID));
                if (service) {
                    rx = service->getCharacteristic(BLEUUID(EXO_RX_UUID));
                    auto tx = service->getCharacteristic(BLEUUID(EXO_TX_UUID));
                    if (rx && rx->canWrite() && tx && tx->canNotify()) {
                        tx->registerForNotify(notified);
                        ready = client->isConnected();
                    }
                }
            }
            s.connected = ready;
            s.busy = false;
            if (ready) {
                copyText(s.peer, s.devices[c.index].name);
                copyText(s.message, "Connecte. Test PING disponible.");
            } else {
                if (client->isConnected()) client->disconnect();
                rx = nullptr;
                copyText(s.peer, "");
                copyText(s.message, "Echec connexion ou service incompatible.");
            }
            publish(s);
        }
        else if (c.type == 3) {
            ready = false;
            rx = nullptr;
            if (client->isConnected()) client->disconnect();
            s.connected = false;
            copyText(s.peer, "");
            copyText(s.state, "Inconnu");
            copyText(s.message, "Deconnecte.");
            publish(s);
        }
        else if (c.type == 4) {
            if (ready && client->isConnected() && rx) {
                rx->writeValue(reinterpret_cast<uint8_t*>(c.text), strlen(c.text), true);
                copyText(s.message, "Message envoye; attendre la reponse.");
            } else {
                copyText(s.message, "Pas de connexion active.");
            }
            publish(s);
        }
    }
}

bool enqueue(int type, int index, const char* text) {
    if (!commands) return false;
    Command c = {};
    c.type = type;
    c.index = index;
    copyText(c.text, text);
    return xQueueSend(commands, &c, 0) == pdTRUE;
}
}

void exoBegin() {
    commands = xQueueCreate(4, sizeof(Command));
    incoming = xQueueCreate(8, sizeof(Incoming));
    if (!commands || !incoming) {
        Serial.println("Erreur allocation files BLE");
        return;
    }
    if (xTaskCreate(worker, "exo_ble", 8192, nullptr, 1, nullptr) != pdPASS) {
        Serial.println("Erreur creation tache BLE");
    }
}

ExoSnapshot exoSnapshot() {
    ExoSnapshot s;
    portENTER_CRITICAL(&stateLock);
    s = shared;
    portEXIT_CRITICAL(&stateLock);
    return s;
}
bool exoScan() { return enqueue(1, 0, ""); }
bool exoConnect(int index) { return enqueue(2, index, ""); }
bool exoDisconnect() { return enqueue(3, 0, ""); }
bool exoSend(const char* text) {
    return text && strlen(text)<=20 && enqueue(4, 0, text);
}
