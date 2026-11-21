#pragma once
#include <Arduino.h>

#define EXO_SERVICE_UUID \
    "19b10000-e8f2-537e-4f6c-d104768a1214"

// Montre → Nano : la montre écrit dans cette caractéristique.
#define EXO_RX_UUID \
    "19b10002-e8f2-537e-4f6c-d104768a1214"

// Nano → montre : la montre s’abonne aux notifications.
#define EXO_TX_UUID \
    "19b10001-e8f2-537e-4f6c-d104768a1214"

struct ExoDeviceInfo {
    char name[32];
    int rssi;
};

struct ExoSnapshot {
    uint32_t revision;
    bool busy;
    bool connected;
    int count;
    ExoDeviceInfo devices[4];
    char peer[32];
    char state[32];
    char message[64];
};

void exoBegin();
ExoSnapshot exoSnapshot();
bool exoScan();
bool exoConnect(int index);
bool exoDisconnect();
// true = commande mise en file, pas confirmation de reception distante.
bool exoSend(const char* message);
