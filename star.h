#include "space.h"
class STAR {
    int x, y;
    SPACE* space;
public:
    void draw();
    void move(int enemyfirstrow);
    COORD getPos();
    STAR(SPACE* _s, int _x, int _y) : space(_s), x(_x), y(_y) {}
};
COORD STAR::getPos() {
    COORD c;
    c.X = this->x;
    c.Y = this->y;
    return c;
}
void STAR::draw() {
    this->space->gotoxy(this->x, this->y); std::cout << (char)249;
}
void STAR::move(int enemyfirstrow) {
    this->space->gotoxy(this->x, this->y); std::cout << " ";
    this->y += 1;
    if (this->y == this->space->limit_bottom - 1) {
        this->x = rand() % (this->space->limit_right - 1) + 1;
        this->y = this->space->limit_top + enemyfirstrow * 3; // *3 (size of enemy)
    }
    this->draw();
}


