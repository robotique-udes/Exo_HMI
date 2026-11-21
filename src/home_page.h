#pragma once
#include <Arduino.h>
enum class HomeAction { None, Bluetooth, Profile, Settings, Debug };
void drawHomePage(const String& peer = "", const String& state = "Inconnu");
void setHomeMessage(const String& message);
HomeAction getHomeAction(int x, int y);
