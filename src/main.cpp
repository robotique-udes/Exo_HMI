#include <Arduino.h>
#include <M5Core2.h>
#include "home_page.h"
#include "bluetooth_page.h"
#include "ble_link.h"

namespace {
bool bluetoothPage=false;
bool wasTouched=false;
uint32_t lastRevision=UINT32_MAX;
void render(const ExoSnapshot& s) {
    if(bluetoothPage) drawBluetoothPage(s);
    else drawHomePage(s.connected ? String(s.peer) : String(""),String(s.state));
    lastRevision=s.revision;
}
}
void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    exoBegin();
    render(exoSnapshot());
}
void loop() {
    M5.update();
    ExoSnapshot s=exoSnapshot();
    if(s.revision!=lastRevision) render(s);
    TouchPoint_t p=M5.Touch.getPressPoint();
    bool touched=p.x>=0 && p.y>=0;
    if(touched && !wasTouched) {
        if(bluetoothPage) {
            if(touchBluetoothPage(p.x,p.y,s)) {
                bluetoothPage=false;
                render(s);
            }
        } else {
            switch(getHomeAction(p.x,p.y)) {
                case HomeAction::Bluetooth:
                    bluetoothPage=true;
                    render(s);
                    break;
                case HomeAction::Profile: setHomeMessage("Page profil a venir"); break;
                case HomeAction::Settings: setHomeMessage("Page reglages a venir"); break;
                case HomeAction::Debug: setHomeMessage("Page debug a venir"); break;
                default: break;
            }
        }
    }
    wasTouched=touched;
    delay(10);
}
