#include <M5Core2.h>
#include <BluetoothSerial.h>
#include "bluetooth_page.h"
#include "ui.h"

BluetoothSerial Bluetooth;

void drawBluetoothPage() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);

    M5.Lcd.setCursor(20, 15);
    M5.Lcd.println("Bluetooth");

    M5.Lcd.fillRoundRect(70, 65, 180, 55, 8, BLUE);
    M5.Lcd.setCursor(95, 85);
    M5.Lcd.println("Rechercher");

    M5.Lcd.fillRoundRect(10, 195, 80, 35, 6, DARKGREY);
    M5.Lcd.setCursor(25, 205);
    M5.Lcd.println("Retour");

    if (!Bluetooth.hasClient()) {
        M5.Lcd.setCursor(20, 145);
        M5.Lcd.println("Aucun appareil connecte");
    }
}

bool isScanBluetoothButton(int x, int y) {
    return x >= 70 && x <= 250 && y >= 65 && y <= 120;
}

void scanBluetoothDevices() {
    M5.Lcd.fillRect(10, 140, 300, 40, BLACK);
    M5.Lcd.setCursor(20, 145);
    M5.Lcd.println("Bluetooth actif");

    if (!Bluetooth.hasClient()) {
        M5.Lcd.setCursor(20, 170);
        M5.Lcd.println("Aucun appareil connecte");
    }
}