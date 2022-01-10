
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include "space.h"
#include "bullet.h"
#include "ship.h"
#include "asteroid.h"
#include "star.h"
#include "enemy.h"

using namespace std;

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
