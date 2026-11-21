#include <M5Core2.h>
#include "ui.h"

void drawHomePage() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);

    M5.Lcd.setCursor(20, 15);
    M5.Lcd.println("Menu principal");

    M5.Lcd.fillRoundRect(20, 55, 130, 55, 8, BLUE);
    M5.Lcd.setCursor(48, 75);
    M5.Lcd.println("Bluetooth");

    M5.Lcd.fillRoundRect(170, 55, 130, 55, 8, CYAN);
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.setCursor(215, 75);
    M5.Lcd.println("WiFi");

    M5.Lcd.fillRoundRect(80, 140, 160, 55, 8, RED);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setCursor(115, 160);
    M5.Lcd.println("Fermer");
}

bool isCloseButton(int x, int y) {
    return x >= 80 && x <= 240 && y >= 140 && y <= 195;
}

bool isBluetoothButton(int x, int y) {
    return x >= 20 && x <= 150 && y >= 55 && y <= 110;
}

bool isWifiButton(int x, int y) {
    return x >= 170 && x <= 300 && y >= 55 && y <= 110;
}

bool isBackButton(int x, int y) {
    return x >= 0 && x <= 90 && y >= 195 && y <= 235;
}

void shutdownScreen() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(50, 100);
    M5.Lcd.println("Ecran eteint");

    delay(500);

    M5.Lcd.sleep();
}