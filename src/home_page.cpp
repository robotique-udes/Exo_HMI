#include <M5Core2.h>
#include "home_page.h"
namespace {
constexpr uint16_t UI_BG=0x0841, UI_PANEL=0x10A2, UI_ACCENT=0xFD20;
constexpr uint16_t UI_TEXT=0xEF5D, UI_DIM=0x9492, UI_OK=0x07D0;
struct HomeButtonLayout {
    int x,y,w,h;
    const char* label;
    HomeAction action;
};
const HomeButtonLayout kHomeButtons[]={
    {12,132,144,38,"Bluetooth",HomeAction::Bluetooth},
    {164,132,144,38,"Profil",HomeAction::Profile},
    {12,178,144,38,"Reglages",HomeAction::Settings},
    {164,178,144,38,"Debug",HomeAction::Debug}
};
void text(const String& value,int x,int y,int size,uint16_t color,uint16_t bg) {
    M5.Lcd.setTextFont(1);
    M5.Lcd.setTextSize(size);
    M5.Lcd.setTextColor(color,bg);
    M5.Lcd.setCursor(x,y);
    M5.Lcd.print(value);
}
}
void drawHomePage(const String& peer,const String& state) {
    M5.Lcd.fillScreen(UI_BG);
    M5.Lcd.setTextWrap(false);
    text("BIOGENIUS",12,12,2,UI_ACCENT,UI_BG);
    text("ACCUEIL",245,17,1,UI_DIM,UI_BG);
    M5.Lcd.drawFastHLine(12,39,296,UI_ACCENT);
    M5.Lcd.fillRoundRect(12,50,296,70,5,UI_PANEL);
    M5.Lcd.drawRoundRect(12,50,296,70,5,0x39C7);
    M5.Lcd.fillCircle(25,67,3,peer.length() ? UI_OK : UI_DIM);
    text("INTERFACE PRETE",36,63,1,UI_OK,UI_PANEL);
    String bt = peer.length() ? "Connecte : "+peer : "Bluetooth : non connecte";
    text(bt.substring(0,45),24,83,1,UI_TEXT,UI_PANEL);
    text(("Etat : "+state).substring(0,45),24,101,1,UI_DIM,UI_PANEL);
    for (const HomeButtonLayout& b:kHomeButtons) {
        M5.Lcd.fillRoundRect(b.x,b.y,b.w,b.h,5,UI_PANEL);
        M5.Lcd.drawRoundRect(b.x,b.y,b.w,b.h,5,UI_ACCENT);
        M5.Lcd.fillRect(b.x+1,b.y+10,3,b.h-20,UI_ACCENT);
        M5.Lcd.setTextFont(1);
        M5.Lcd.setTextSize(2);
        int tx=b.x+(b.w-M5.Lcd.textWidth(b.label))/2;
        int ty=b.y+(b.h-M5.Lcd.fontHeight())/2;
        text(b.label,tx,ty,2,UI_ACCENT,UI_PANEL);
    }
    setHomeMessage("Choisir une fonction");
}
void setHomeMessage(const String& message) {
    M5.Lcd.fillRect(0,224,320,16,UI_BG);
    text(message.substring(0,49),12,224,1,UI_DIM,UI_BG);
}
HomeAction getHomeAction(int x,int y) {
    for (const HomeButtonLayout& b:kHomeButtons) {
        if(x>=b.x && x<b.x+b.w && y>=b.y && y<b.y+b.h) return b.action;
    }
    return HomeAction::None;
}
