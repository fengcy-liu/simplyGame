#pragma once
#include <graphics.h>
#include "point.h"

enum class renderSts
{
    IDLE = 0,
    PROCESS,
};

class render
{
public:
    static renderSts renderStatus;
    static void drawBoxNum(point pos, int size, int id, COLORREF boxColor = DARKGRAY);
    static void drawMessage(LPCTSTR message, point pos, point size, COLORREF boxColor = DARKGRAY);
    static void PushMessage(LPCTSTR msg, RECT* rect, int msgSize);
    static void drawSquare(int x, int y, int length, COLORREF boxColor = BLUE);
    static void drawRect(int x, int y, int xlen, int ylen, COLORREF boxColor = GREEN);
    static void begin();
    static void end();
    static void flush();
};
