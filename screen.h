#pragma once
#include <iostream>
#include <string>
#include <windows.h>

typedef unsigned char byte;

/*
Possible ratios
 4 : 6
 6 : 8
 8 : 8
16 : 8
 5 : 12
 7 : 12
16 : 12
12 : 16
10 : 18
*/
constexpr int dW = 8, dH = 8;

int WIDTH = GetSystemMetrics(SM_CXSCREEN) - dW;
int HEIGHT = GetSystemMetrics(SM_CYSCREEN) - dH;

class Screen {
public:
    Screen(){
        Setup();
        Clear();
    }

    void Clear() {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                canvas[i* WIDTH +j] = 0;
            }
        }
    }

    void DrawPoint(int A, int B, byte luminance = 255) {
        if (A < 0 || B < 0 || A >= HEIGHT || B >= WIDTH) {
            return;
        }

        canvas[A * WIDTH + B] = luminance;
    }

    void Draw() {
        //allocate
        int Y = HEIGHT / dH, X = WIDTH / dW + 1;
     
        char* frame=new char[Y*X];

        for (int i = 0; i < Y - 1; ++i) {
            frame[X * i + X - 1] = '\n';
        }
        frame[X * (Y - 1) + X - 1] = '\0';

        for (int i = 0; i < Y; i++) {
            for (int j = 0; j < X-1; j++) {
                int count = 0;

                // calculating brightness
                for (int k = 0; k < dH; k++) {
                    for (int l = 0; l < dW; l++) {
                        count += canvas[(dH * i + k)* WIDTH +(dW * j + l)];
                    }
                }
                frame[X*i+j] = brightness(count);
            }
        }
        // borders
        for (int i = 0; i < Y; ++i) {
            frame[X * i] = '@';
            frame[X * i + X - 2] = '@';
        }
        for (int j = 0; j < X-1; ++j) {
            frame[j] = '@';
            frame[X*(Y-1)+j] = '@';
        }
        FillScreenWithString(frame);
        delete[] frame;
    }

    int Height() const {
        return HEIGHT;
    }

    int Width() const {
        return WIDTH;
    }

    void set_palette(int palette) {
        this->_palette = palette;
    }

    ~Screen()
    {
        delete[] canvas;
    }
private:
    byte* canvas=new byte[HEIGHT*WIDTH];
    int _palette = 0;

    void Setup();
    void FillScreenWithString(const char *frame);

    char brightness(int count) const {
        static const struct {
            int n;
            const char s[19];
        } p[] = {
            {7, " .:+%#@"},
            {18, " .,^:-+abcdwf$&%#@"},
            {3, " .:"},
        };
        if (0 <= _palette && _palette <= 2) {
            const auto &pal = p[_palette];
            return pal.s[count * (pal.n) / 256 / dW / dH];
        } else {
            return ' ';
        }
    }
};


void Screen::FillScreenWithString(const char *frame) {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    fputs(frame,stdout);
}

void Screen::Setup() {
    //this function is a mess, just ignore it

    CONSOLE_FONT_INFOEX cf = {0};
    cf.cbSize = sizeof cf;
    cf.dwFontSize.X = dW;
    cf.dwFontSize.Y = dH;
    wcscpy_s(cf.FaceName, L"Terminal");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), 0, &cf);

    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hstdout, &csbi);

    csbi.dwSize.X = csbi.dwMaximumWindowSize.X;
    csbi.dwSize.Y = csbi.dwMaximumWindowSize.Y;
    SetConsoleScreenBufferSize(hstdout, csbi.dwSize);
    MoveWindow(console, 0, 0, WIDTH, HEIGHT, TRUE);
    SetConsoleDisplayMode(hstdout, CONSOLE_FULLSCREEN_MODE, 0);
    ShowScrollBar(console, SB_BOTH, FALSE);
}
