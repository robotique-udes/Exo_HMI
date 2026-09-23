#include <Arduino.h>
#include <M5Core2.h>
#include "ui.h"
#include "bluetooth_page.h"
#include "wifi_page.h"

enum Page {
    PAGE_HOME,
    PAGE_BLUETOOTH,
    PAGE_WIFI
};

Page currentPage = PAGE_HOME;

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);

    drawHomePage();
}

void loop() {
    M5.update();

    TouchPoint_t touch = M5.Touch.getPressPoint();

    if (touch.x < 0 || touch.y < 0) {
        return;
    }

    if (currentPage == PAGE_HOME) {
        if (isCloseButton(touch.x, touch.y)) {
            shutdownScreen();
        }
        else if (isBluetoothButton(touch.x, touch.y)) {
            currentPage = PAGE_BLUETOOTH;
            drawBluetoothPage();
        }
        else if (isWifiButton(touch.x, touch.y)) {
            currentPage = PAGE_WIFI;
            drawWifiPage();
        }
    }
    else if (currentPage == PAGE_BLUETOOTH) {
        if (isBackButton(touch.x, touch.y)) {
            currentPage = PAGE_HOME;
            drawHomePage();
        }
        else if (isScanBluetoothButton(touch.x, touch.y)) {
            scanBluetoothDevices();
        }
    }
    else if (currentPage == PAGE_WIFI) {
        if (isBackButton(touch.x, touch.y)) {
            currentPage = PAGE_HOME;
            drawHomePage();
        }
        else if (isScanWifiButton(touch.x, touch.y)) {
            scanWifiNetworks();
        }
    }

    delay(150);
}