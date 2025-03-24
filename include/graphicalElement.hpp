#ifndef GRAPHICAL_ELEMENT_HPP
#define GRAPHICAL_ELEMENT_HPP

#include <LilyGoLib.h>
#include <TFT_eWidget.h>
#include <string>
#include "position.hpp"
#include "eventListener.hpp"
#include "state.hpp"


// Base class of all graphical elements such as buttons, sliders, switches etc...
class GraphicalElement{
public:

    GraphicalElement(Position position, Position dimensions, State state, EventListener* eventListener, std::string elementName, LilyGoLib* watch);

    void update(Position touchLocation);

    void draw();
    
private:

    // Boutton vs Simple text
    bool isInteractive; 

    Position position;
    Position dimensions; // Pas terrible

    State elementState;

    // If the element has text, this is where it goes.
    std::string elementString;

    std::string elementName;

    EventListener* eventListener;

    LilyGoLib* drawWatch;

    ButtonWidget* btn;


};

#endif