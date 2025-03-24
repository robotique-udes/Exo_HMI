#ifndef EVENT_LISTENER_HPP
#define EVENT_LISTENER_HPP

#include <queue>
#include "event.hpp"

class EventListener{

public:
    EventListener();

    // Call once a loop. Sends required info via wifi
    void update();

    // Adds an event to the queue for processing
    void addNewEvent(Event newEvent);


private:

    std::queue<Event> eventQueue;
};

#endif