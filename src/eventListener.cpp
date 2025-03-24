#include "eventListener.hpp"

EventListener::EventListener(){}

void EventListener::update(){
    // Iterate through the eventQueue and send them via wifi/bluetooth to the exo
    while (!eventQueue.empty()) {
        // For now, just removing it as println seems to complicate flashing process...
        eventQueue.pop();
    }
}

void EventListener::addNewEvent(Event newEvent){
    eventQueue.push(newEvent);
}