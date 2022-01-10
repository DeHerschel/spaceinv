#include "space.h"
class ENEMY {
    int x, y;
    int health = 3;
    SPACE* space;
public:
    COORD getPos();
    void downHealth();
    int getHealth();
    void remove();
    void move();
    void draw();
    ENEMY(SPACE* _s, int _x, int _y) : space(_s), x(_x), y(_y) {};
};
COORD ENEMY::getPos() {
    COORD c;
    c.X = this->x;
    c.Y = this->y;
    return c;
}
void ENEMY::downHealth() {
    this->health--;
}
int ENEMY::getHealth() {
    return this->health;
}
void ENEMY::draw() {
    space->gotoxy(this->x, this->y); std::cout << (char)47 << (char)178 << (char)178 << (char)178 << (char)92;
    space->gotoxy(this->x, this->y + 1); std::cout << (char)92 << "_V_/";
}
void ENEMY::move() {
    this->y += 3;
}
void ENEMY::remove() {
    space->gotoxy(this->x, this->y); std::cout << "     ";
    space->gotoxy(this->x, this->y + 1); std::cout << "     ";
}
