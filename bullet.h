#pragma once
#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include "space.h"
#include "bullet.h"
class BULLET {
    int x, y;
    SPACE* space;
public:
    COORD getPos();
    void move();
    BULLET(int _x, int _y, SPACE* _space) : x(_x), y(_y), space(_space) { }
};
COORD BULLET::getPos() {
    COORD c;
    c.X = this->x;
    c.Y = this->y;
    return c;
}
void BULLET::move() {
    space->gotoxy(this->x, this->y); std::cout << " ";
    this->y--;
    space->gotoxy(this->x, this->y); std::cout << "|";
}

