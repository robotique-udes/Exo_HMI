#include "graphicalElement.hpp"

GraphicalElement::GraphicalElement(Position position, Position dimensions, State state, EventListener* eventListener, std::string elementName, LilyGoLib* watch):
    position(position),
    dimensions(dimensions),
    elementState(state),
    eventListener(eventListener),
    elementName(elementName),
    drawWatch(watch)
    {
        btn = new ButtonWidget(watch);
        btn->initButtonUL(position.x, position.y, dimensions.x, dimensions.y, TFT_WHITE, TFT_RED, TFT_BLACK, "Button", 1);
        btn->setPressAction(NULL);
        btn->setReleaseAction(NULL);
        btn->drawSmoothButton(false, 3, TFT_BLACK);
    }

void GraphicalElement::update(Position touchLocation){
    if(touchLocation.x == -1 && touchLocation.y == -1){
        return;
    }

    // TODO : Write the condition to check if the touch is on the element
    if(true){
        eventListener->addNewEvent(Event(elementName, touchLocation, State::EXO_PRESSED, 0));
    }

}

void GraphicalElement::draw(){
    // TODO : Figure out the way to do this ... 
    // Pourrait être ave la portion eWidet de tft ou bien
    // simplement avec des draw rect primitifs ...
    // a voir ce qu'on veut faire
}