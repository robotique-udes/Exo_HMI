#ifndef TOUCH_HPP
#define TOUCH_HPP

#include <LilyGoLib.h>
#include "position.hpp"

class Touch{
public:

    Touch(LilyGoLib* touchScreen);

    bool isBeingTouched();
    Position getPosition();

private:

    Position lastTouchPosition;
    LilyGoLib* touchScreen;


};

#endif
