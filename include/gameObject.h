#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include "unit.h"
#include "icon.h"
#include <iostream>
class GameObject{

public:

    Position getPosition() const;
    Icon getIcon() const;
    void setPosition(const Position& pos);
    void setIcon(const Icon& icon);
    virtual void update();
    void move(int dx, int dy);
    virtual std::string getType() const = 0;
protected:

    Position _pos;
    Icon _icon;

};

#endif

