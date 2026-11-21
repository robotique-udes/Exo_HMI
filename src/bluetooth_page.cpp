#include <M5Core2.h>
#include "bluetooth_page.h"

namespace {
constexpr uint16_t BG=0x0841, PANEL=0x10A2, ACCENT=0xFD20;
constexpr uint16_t TXT = 0xEF5D;
constexpr uint16_t DIM = 0x9492;
constexpr uint16_t BT_COLOR_CONNECTED = 0x07D0;

void text(const String& value, int x, int y, int size, uint16_t color) {
    M5.Lcd.setTextFont(1);
    M5.Lcd.setTextSize(size);
    M5.Lcd.setTextColor(color, BG);
    M5.Lcd.setCursor(x,y);
    M5.Lcd.print(value);
}
String shortText(const char* value, unsigned int limit) {
    String s(value);
    return s.length()>limit ? s.substring(0,limit-2)+".." : s;
}
void button(int x, int w, const char* label) {
    M5.Lcd.fillRoundRect(x,202,w,30,4,PANEL);
    M5.Lcd.drawRoundRect(x,202,w,30,4,ACCENT);
    M5.Lcd.setTextFont(1);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(ACCENT,PANEL);
    M5.Lcd.setCursor(x+(w-M5.Lcd.textWidth(label))/2,213);
    M5.Lcd.print(label);
}
}

void drawBluetoothPage(const ExoSnapshot& s) {
    M5.Lcd.fillScreen(BG);
    M5.Lcd.setTextWrap(false);
    text("BLUETOOTH",12,10,2,ACCENT);
    text(
        s.connected ? "CONNECTE" : "HORS LIGNE",
        242, 17, 1,
        s.connected ? BT_COLOR_CONNECTED : DIM
    );    M5.Lcd.drawFastHLine(12,35,296,ACCENT);

    if (s.connected) {
        text("Appareil connecte",12,48,1,DIM);
        text(
        shortText(s.peer, 24),
        12, 66, 2,
        BT_COLOR_CONNECTED
        );
        text("Dernier etat recu :",12,97,1,DIM);
        text(shortText(s.state,24),12,114,2,TXT);
        text("Test envoi : bouton PING",12,149,1,DIM);
    } else if (!s.busy && s.count>0) {
        for (int i=0;i<s.count;++i) {
            int y=44+i*30;
            M5.Lcd.fillRoundRect(12,y,296,26,4,PANEL);
            M5.Lcd.drawRoundRect(12,y,296,26,4,ACCENT);
            M5.Lcd.setTextFont(1);
            M5.Lcd.setTextSize(1);
            M5.Lcd.setTextColor(TXT,PANEL);
            M5.Lcd.setCursor(20,y+9);
            M5.Lcd.print(shortText(s.devices[i].name,34));
            M5.Lcd.setCursor(266,y+9);
            M5.Lcd.printf("%d",s.devices[i].rssi);
        }
    } else {
        text(s.busy ? "Operation en cours..." : "Rechercher votre ESP32",12,65,2,TXT);
        text("Filtre : service BLE du projet",12,102,1,DIM);
    }
    // Deux lignes pour les messages longs, sans chevauchement des boutons.
    String msg(s.message);
    text(msg.substring(0,48),12,174,1,DIM);
    text(msg.substring(48,96),12,185,1,DIM);
    button(12,88,"Retour");
    button(108,96,s.connected ? "PING" : "Chercher");
    button(212,96,"Deconnecter");
}

bool touchBluetoothPage(int x, int y, const ExoSnapshot& s) {
    if (y>=202 && y<232) {
        if (x>=12 && x<100) return true;
        if (s.busy) return false;
        if (x>=108 && x<204) {
            if (s.connected) exoSend("PING");
            else exoScan();
        } else if (x>=212 && x<308 && s.connected) exoDisconnect();
    }
    if (!s.busy && !s.connected && x>=12 && x<308) {
        for (int i=0;i<s.count;++i) {
            int top=44+i*30;
            if (y>=top && y<top+26) { exoConnect(i); break; }
        }
    }
    return false;
}
