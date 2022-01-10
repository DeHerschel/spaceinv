#pragma once

#include <Windows.h>
#include <iostream>

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
