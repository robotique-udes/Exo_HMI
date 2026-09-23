#include <M5Core2.h>
#include <WiFi.h>
#include "wifi_page.h"

void drawWifiPage() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);

    M5.Lcd.setCursor(20, 15);
    M5.Lcd.println("WiFi");

    M5.Lcd.fillRoundRect(70, 65, 180, 55, 8, CYAN);
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.setCursor(95, 85);
    M5.Lcd.println("Rechercher");

    M5.Lcd.fillRoundRect(10, 195, 80, 35, 6, DARKGREY);
    M5.Lcd.setCursor(25, 205);
    M5.Lcd.println("Retour");
}

bool isScanWifiButton(int x, int y) {
    return x >= 70 && x <= 250 && y >= 65 && y <= 120;
}

void scanWifiNetworks() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(1);

    M5.Lcd.setCursor(10, 10);
    M5.Lcd.println("Reseaux WiFi:");

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    int networkCount = WiFi.scanNetworks();

    if (networkCount == 0) {
        M5.Lcd.setCursor(10, 35);
        M5.Lcd.println("Aucun reseau trouve");
        return;
    }

    for (int i = 0; i < networkCount && i < 8; i++) {
        M5.Lcd.setCursor(10, 30 + i * 22);
        M5.Lcd.printf("%d: %s (%d dBm)",
                      i + 1,
                      WiFi.SSID(i).c_str(),
                      WiFi.RSSI(i));
    }
}