#include "touch.hpp"

Touch::Touch(LilyGoLib* touchScreen): touchScreen(touchScreen){}

bool Touch::isBeingTouched(){
    // TODO : Figure out how to get the position
    lastTouchPosition = Position(40,40);
    return touchScreen->getTouched();
}

Position Touch::getPosition(){
    return lastTouchPosition;
}