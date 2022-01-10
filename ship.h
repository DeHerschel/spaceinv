#pragma once
#include <stdlib.h>
#include <list>
#include "bullet.h"

using namespace std;

class SHIP {
private:
    SPACE* space;
    int x, y; //pos
    int health;
    int lives;
    void detectLimitCols();
    void draw();
    list<BULLET*>* BULLETS;
public:
    int getLives();
    COORD getPosition();
    void healthDown();
    void drawLives();
    void move();
    void shoot();
    void dead();
    void remove();
    SHIP(class SPACE* _space, int _x, int _y, int _lives, int _health, list<BULLET*>* _BULLETS) : space(_space), x(_x), y(_y), lives(_lives), health(_health), BULLETS(_BULLETS) { draw(); };
};
int SHIP::getLives() {
    return this->lives;
}
void SHIP::draw() {
    space->gotoxy(this->x, this->y); std::cout << (char)32 << (char)32 << (char)30;
    space->gotoxy(this->x, this->y + 1); std::cout << (char)32 << (char)40 << (char)207 << (char)41;
    space->gotoxy(this->x, this->y + 2); std::cout << (char)30 << (char)190 << (char)32 << char(190) << (char)30;
}
void SHIP::detectLimitCols() {
    if (this->y == this->space->limit_top) this->y++;
    if (this->y + 2 == this->space->limit_bottom) this->y--;
    if (this->x == this->space->limit_left) this->x++;
    if (this->x + 4 == this->space->limit_right) this->x--;
}
void SHIP::healthDown() {
    health--;
}
COORD SHIP::getPosition() {
    COORD c;
    c.X = this->x;
    c.Y = this->y;
    return c;
}
void SHIP::remove() {
    space->gotoxy(this->x, this->y); std::cout << "     ";
    space->gotoxy(this->x, this->y + 1); std::cout << "     ";
    space->gotoxy(this->x, this->y + 2); std::cout << "     ";
}
void SHIP::move() {
    if (GetKeyState('L') & 0x8000) { this->remove(); this->x++; }
    if (GetKeyState('J') & 0x8000) { this->remove(); this->x--; }
    if (GetKeyState('I') & 0x8000) { this->remove(); this->y--; }
    if (GetKeyState('K') & 0x8000) { this->remove(); this->y++; }
    if (GetKeyState('E') & 0x8000) this->shoot();
    this->detectLimitCols();
    this->drawLives();
    this->draw();
}
void SHIP::shoot() {
    BULLETS->push_back(new BULLET(this->x + 2, this->y - 1, space));
}

void SHIP::drawLives() {
    space->gotoxy(33, 37); std::cout << "        ";
    space->gotoxy(17, 37); std::cout << "     ";
    space->gotoxy(10, 37); std::cout << "Lives:" << std::endl;
    space->gotoxy(17, 37); std::cout << lives << std::endl;
    space->gotoxy(25, 37); std::cout << "Health:" << std::endl;
    for (int i = 0; i < this->health; i++) {
        space->gotoxy(33 + i * 2, 37); std::cout << (char)3;
    }
}
void SHIP::dead() {
    if (this->health == 0) {
        this->remove();
        space->gotoxy(this->x, this->y); std::cout << " **";
        space->gotoxy(this->x, this->y + 1); std::cout << "****";
        space->gotoxy(this->x, this->y + 2); std::cout << " **";
        Sleep(200);
        space->gotoxy(this->x, this->y); std::cout << "* * *";
        space->gotoxy(this->x, this->y + 1); std::cout << " ***";
        space->gotoxy(this->x, this->y + 2); std::cout << "* * *";
        Sleep(200);
        this->lives--;
        this->health = 3;
        this->remove();
        this->draw();
    }
}
