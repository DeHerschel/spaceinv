#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <iostream>
#include <list>

using namespace std;

HANDLE hCon;

class SPACE {
private:
    int w, h;
    void resizeConsole();
    void drawLimits();
    void hideCursor();
    void draw();
public:
    // limits y
    int limit_top = 0;
    int limit_bottom = h - 5;
    //limits x
    int limit_right = w - 1;
    int limit_left = 0;
    void gotoxy(int _x, int _y);
    SPACE(int _w, int _h) : w(_w), h(_h) { draw(); }
};
void SPACE::gotoxy(int x, int y) {
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hCon, dwPos);
}
void SPACE::resizeConsole() {
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = { 0, 0, this->w - 1, this->h - 1 };
    SetConsoleWindowInfo(hCon, TRUE, &windowSize);
    COORD c;
    c.X = this->w;
    c.Y = this->h;
    SetConsoleScreenBufferSize(hCon, c);
}
void SPACE::drawLimits() {
    for (int i = 0; i <= this->limit_right; i++) {
        gotoxy(i, this->limit_top); std::cout << (char)205;
        gotoxy(i, this->limit_bottom); std::cout << (char)205;
    }
    for (int j = 0; j <= this->limit_bottom; j++) {
        gotoxy(this->limit_left, j); std::cout << (char)186;
        gotoxy(this->limit_right, j); std::cout << (char)186;
    }
    gotoxy(this->limit_left, this->limit_top); std::cout << (char)201;
    gotoxy(this->limit_left, this->limit_bottom); std::cout << (char)200;
    gotoxy(this->limit_right, this->limit_top); std::cout << (char)187;
    gotoxy(this->limit_right, this->limit_bottom); std::cout << (char)188;
}
void SPACE::hideCursor() {
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 2;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hCon, &cci);
}
void SPACE::draw() {
    this->resizeConsole();
    this->drawLimits();
    this->hideCursor();
}
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
int main() {
    SPACE space(100, 40);
    list<BULLET*> BULLETS;
    list<BULLET*>::iterator it;
    list<ASTEROID*> ASTEROIDS;
    list<ASTEROID*>::iterator itAst;
    list<STAR*> STARS;
    list<STAR*>::iterator itStar;
    list<ENEMY*> ENEMYS;
    list<ENEMY*>::iterator itEne;
    int nasteroids = 5;
    int stars = 10;
    int enemyfirstrow = 1;
    for (int i = 0; i < nasteroids; i++) {
        ASTEROIDS.push_back(new ASTEROID(&space, rand() % (space.limit_right - 1) + 1, rand() % (space.limit_top + 10) + 1, enemyfirstrow));
    }
    for (int i = 0; i < stars; i++) {
        STARS.push_back(new STAR(&space, rand() % (space.limit_right - 1) + 1, rand() % (space.limit_bottom - 1) + 1));
    }
    SHIP ship(&space, 58, 30, 3, 3, &BULLETS); //x, y, space, lives, health, bullets
    int enemies = 0;
    for (int i = 2; i < space.limit_right - 2; i += 6) {
        ENEMYS.push_back(new ENEMY(&space, i, 1));
        enemies++;
    }
    bool game_over = false;
    int points = 0;
    int count = 0; // Counts iterations to move enemys
    int dead = 0; 
    while (!game_over) {
        space.gotoxy(40, 37); std::cout << "     ";
        space.gotoxy(45, 37); std::cout << "Points" << std::endl;
        space.gotoxy(52, 37); std::cout << points << std::endl;
        for (itEne = ENEMYS.begin(); itEne != ENEMYS.end(); itEne++) {
            (*itEne)->draw();
            if (count == 80) {
                (*itEne)->move();
            }
            if (count == 80 + enemies) {
                count = 0;
                for (int i = 2; i < space.limit_right - 2; i += 6) {
                    ENEMYS.push_back(new ENEMY(&space, i, 1));
                }
                enemyfirstrow++;

            }

        }

        for (it = BULLETS.begin(); it != BULLETS.end();) {
            (*it)->move();
            for (itEne = ENEMYS.begin(); itEne != ENEMYS.end(); itEne++) {
                if ((*it)->getPos().X >= (*itEne)->getPos().X && (*it)->getPos().X <= (*itEne)->getPos().X + 5) {
                    if ((*it)->getPos().Y - 1 == (*itEne)->getPos().Y) {
                        space.gotoxy((*it)->getPos().X, (*it)->getPos().Y); std::cout << " ";
                        delete (*it);
                        it = BULLETS.erase(it);

                        (*itEne)->downHealth();

                        if ((*itEne)->getHealth() == 0) {
                            (*itEne)->remove();
                            delete(*itEne);
                            itEne = ENEMYS.erase(itEne);
                            points += 50;
                            dead++;
                        }
                     
                    }
                }
            }
            if ((*it)->getPos().Y == space.limit_top) {
                space.gotoxy((*it)->getPos().X, (*it)->getPos().Y); std::cout << (char)205;
                delete (*it);
                it = BULLETS.erase(it);
            }
            else {
                it++;
            }
        }
        for (itAst = ASTEROIDS.begin(); itAst != ASTEROIDS.end(); itAst++) {
            (*itAst)->move();
            (*itAst)->impact(&ship);
            for (it = BULLETS.begin(); it != BULLETS.end(); it++) {
                if ((*it)->getPos().X == (*itAst)->getPos().X) {
                    if ((*it)->getPos().Y == (*itAst)->getPos().Y) {
                        (*itAst)->destroy();
                        points += 10;
                    }
                    if ((*it)->getPos().Y + 1 == (*itAst)->getPos().Y) {
                        (*itAst)->destroy();
                        points += 10;
                    }
                }
            }
        }
        if (!ship.getLives()) game_over = true;
        ship.dead(); ship.move();
        for (itStar = STARS.begin(); itStar != STARS.end();) {
            (*itStar)->move(enemyfirstrow);
            if ((*itStar)->getPos().Y == space.limit_bottom) {
                space.gotoxy((*itStar)->getPos().X, (*itStar)->getPos().Y); std::cout << (char)205;
                delete (*itStar);
                itStar = STARS.erase(itStar);
            }
            else {
                itStar++;
            }
        }

        count++;
        Sleep(25);
    }
    return 0;
}
