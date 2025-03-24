#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include "position.hpp"
#include "state.hpp"

// Contains the information of an event such as a button press/release
class Event{
public:

    Event(std::string elementName, Position eventPosition, State newState, double eventValue);
    
    std::string getElementName();
    Position getEventPosition();
    State getEventState();
    double getEventValue();

private:

    std::string elementName;
    Position eventPosition;
    State eventState;
    double eventValue;

};

#endif