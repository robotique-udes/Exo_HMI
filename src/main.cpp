#include <Arduino.h>
#include <M5Core2.h>

void setup() {
    M5.begin();

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(20, 40);
    M5.Lcd.println("Test Core2");
}

void loop() {
    M5.update();
}