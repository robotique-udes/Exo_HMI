#include <LilyGoLib.h>

#include "eventListener.hpp"
#include "graphicalElement.hpp"
#include "position.hpp"
#include "touch.hpp"

EventListener eventListener;
GraphicalElement demo(Position(0,0), Position(50,50), State::EXO_ENABLED, &eventListener, "Demo", &watch);
Touch touchManager(&watch);
Position lastTouch;

void setup() {
  watch.begin();
  watch.disableBootDisplay();
  watch.setBrightness(255);
}

void loop() {
  if(touchManager.isBeingTouched()){
    lastTouch = touchManager.getPosition();
  }
  else{
    lastTouch.x = -1;
    lastTouch.y = -1;
  }

  // update all graphical elements
  demo.update(lastTouch);
  demo.draw();


  // then update the eventListener which then sends via wifi to the exo the information
  eventListener.update();
}