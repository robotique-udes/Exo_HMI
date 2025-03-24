#ifndef POSITION_HPP
#define POSITION_HPP

class Position{
public:
    Position(int x, int y): x(x), y(y){}
    Position(): x(0), y(0){}
    int x;
    int y;
};

#endif