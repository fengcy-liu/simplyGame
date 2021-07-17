#include <iostream>
#include "render.h"

LPCTSTR textVal[9] =
{
    _T("*"),
    _T("1"),
    _T("2"),
    _T("3"),
    _T("4"),
    _T("5"),
    _T("6"),
    _T("7"),
    _T("8")
};
renderSts render::renderStatus;

void render::drawMessage(LPCTSTR message, point pos, point size, COLORREF boxColor)
{

    RECT rect = { pos.x, pos.y, pos.x + size.x, pos.y + size.y };
    drawRect(pos.x, pos.y, size.x, size.y, boxColor);
    PushMessage(message, &rect, (int)(30));
}

void render::PushMessage(LPCTSTR msg, RECT* rect, int msgSize)
{
    LOGFONT prefont;
    int preMode;
    gettextstyle(&prefont);

    preMode = getbkmode();
    /* 设置背景透明色 */
    setbkmode(TRANSPARENT);
    settextstyle((int)((float)msgSize), 0, _T("黑体"));
    drawtext(msg, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

    /* 还原之前配置 */
    setbkmode(preMode);
    settextstyle(&prefont);
}

void render::drawBoxNum(point pos, int size, int id, COLORREF boxColor)
{
    RECT rect = { pos.x, pos.y, pos.x + size, pos.y + size };

    render::drawSquare(pos.x, pos.y, size, boxColor);

    PushMessage(textVal[id], &rect, 20);
}

void render::drawSquare(int x, int y, int length, COLORREF boxColor)
{
    int leftx = x;
    int topy = y;
    int rightx = x + length;
    int bottomy = y + length;
    setfillcolor(boxColor);
    fillrectangle(leftx, topy, rightx, bottomy);
    //fillroundrect(leftx, topy, rightx, bottomy, 5, 5);
}

void render::drawRect(int x, int y, int xlen, int ylen, COLORREF boxColor)
{
    int leftx = x;
    int topy = y;
    int rightx = x + xlen;
    int bottomy = y + ylen;
    setfillcolor(boxColor);
    fillrectangle(leftx, topy, rightx, bottomy);
    //fillroundrect(leftx, topy, rightx, bottomy, 5, 5);
}

void render::begin()
{
    if (render::renderStatus == renderSts::IDLE)
    {
        BeginBatchDraw();
        render::renderStatus = renderSts::PROCESS;
    }
}

void render::flush()
{
    if (render::renderStatus == renderSts::PROCESS)
    {
        FlushBatchDraw();
    }
}

void render::end()
{
    if (render::renderStatus == renderSts::PROCESS)
    {
        EndBatchDraw();
        render::renderStatus = renderSts::IDLE;
    }
}
