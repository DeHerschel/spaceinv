#pragma once
#include <stdlib.h>
#include "ship.h"

class ASTEROID {
private:
    int x, y;
    SPACE* space;
    int& enemyrow;
public:
    void draw();
    void move();
    void impact(SHIP* ship);
    void destroy();
    COORD getPos();
    ASTEROID(SPACE* _space, int _x, int _y, int& enemyrow) : space(_space), x(_x), y(_y), enemyrow(enemyrow) {}
};
COORD ASTEROID::getPos() {
    COORD c;
    c.X = this->x;
    c.Y = this->y;
    return c;
}
void ASTEROID::draw() {
    this->space->gotoxy(this->x, this->y); std::cout << "@";
}
void ASTEROID::move() {
    this->space->gotoxy(this->x, this->y); std::cout << " ";
    this->y += 1;
    if (this->y == this->space->limit_bottom - 1) {
        this->x = rand() % (this->space->limit_right - 1) + 1;
        this->y = this->space->limit_top + enemyrow;
    }
    this->draw();
}
void ASTEROID::impact(SHIP* ship) {
    if (this->x >= ship->getPosition().X && this->x <= ship->getPosition().X + 4) {
        if (this->y >= ship->getPosition().Y && this->y <= ship->getPosition().Y + 2) {
            ship->healthDown();
            this->x = rand() % this->space->limit_right + 1;
            this->y = this->space->limit_top + enemyrow;
        }
    }
}
void ASTEROID::destroy() {
    space->gotoxy(this->x, this->y); std::cout << " ";
    this->x = rand() % (this->space->limit_right - 1) + 1;
    this->y = this->space->limit_top + enemyrow;
}
