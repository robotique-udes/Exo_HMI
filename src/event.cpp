#include "event.hpp"

Event::Event(std::string elementName, Position eventPosition, State newState, double eventValue):
    elementName(elementName),
    eventPosition(eventPosition),
    eventState(newState),
    eventValue(eventValue){}
    
std::string Event::getElementName(){
    return elementName;
}

Position Event::getEventPosition(){
    return eventPosition;
}

State Event::getEventState(){
    return eventState;
}

double Event::getEventValue(){
    return eventValue;
}