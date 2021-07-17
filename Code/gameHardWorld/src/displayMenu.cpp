#include "displayMenu.h"

#define START_MENU_SELECT_FORE_COLOR                   FORE_WHITE   
#define START_MENU_SELECT_BACK_COLOR				   BACK_RED   
#define START_MENU_NOT_SELECT_FORE_COLOR               FORE_WHITE   
#define START_MENU_NOT_SELECT_BACK_COLOR			   FORE_DARK     

#ifdef UNICODE
#define LRECTANGLE_LEFT_TOP                             L"\u250F" /* ┏ */
#define LRECTANGLE_RIGHT_TOP                            L"\u2513" /* ┓ */
#define LRECTANGLE_LEFT_BOTTOM                          L"\u2517" /* ┗ */
#define LRECTANGLE_RIGHT_BOTTOM                         L"\u251B" /* ┛ */
#define LRECTANGLE_HORIZONTAL_LINE                      L"\u2501" /* ━ */
#define LRECTANGLE_VERTICAL_LINE                        L"\u2503" /* ┃ */

#define LEMPTY_LINE                                     L" "
#else
#define RECTANGLE_LEFT_TOP                             "\u250F" /* ┏ */
#define RECTANGLE_RIGHT_TOP                            "\u2513" /* ┓ */
#define RECTANGLE_LEFT_BOTTOM                          "\u2517" /* ┗ */
#define RECTANGLE_RIGHT_BOTTOM                         "\u251B" /* ┛ */
#define RECTANGLE_HORIZONTAL_LINE                      "\u2501" /* ━ */
#define RECTANGLE_VERTICAL_LINE                        "\u2503" /* ┃ */

#define EMPTY_LINE                                     " "
#endif // UNICODE
const char_t startMenuInfo[START_MENU_LENGTH][HALF_MAX_BYTES_OF_CHAR] =
{
    L"开始冒险",
    L"加载记录",
    L"设    置",
    L"退    出"
};

COORD startMenuSp = { 56, 7 };
u8_t gap = 2;

COORD fontSize = { 18, 25 };
COORD buffSize = { 100, 40 };


void DM_initConsloe(void)
{

    Console_setConsoleCode(CODE_UNICODE);
    Console_setTitle((char_t*)L"Hard World");
    Console_setFont(fontSize.X, fontSize.Y, 0);
    COORD X;
    Console_SetBuffSize(buffSize);
    Console_hideCursor();
    Console_getBuffSize(&X);
    //Sys_printf("%d, %d\n", X.X, X.Y);
}

void DM_initCfg(void)
{
#if 1
    COORD winSize;
    Console_getBuffSize(&winSize);
    startMenuSp.X = winSize.X / 2 - START_MENU_POSTION_OFFSET;
    startMenuSp.Y = 7;
#endif
}

void DM_initStartMenu()
{
    int i;
    Console_gotoxy(startMenuSp.X, startMenuSp.Y);
    Console_setColor(START_MENU_SELECT_FORE_COLOR, START_MENU_SELECT_BACK_COLOR);
    Sys_printf("%s", startMenuInfo[0]);
    Console_setColor(START_MENU_NOT_SELECT_FORE_COLOR, START_MENU_NOT_SELECT_BACK_COLOR);

    for (i = 1; i < START_MENU_LENGTH; i++)
    {
        Console_gotoxy(startMenuSp.X, startMenuSp.Y + i * gap);
        Sys_printf("%s", startMenuInfo[i]);
    }

}

void DM_updateStartMenu(int preIndex, int curIndex)
{
    Console_gotoxy(startMenuSp.X, startMenuSp.Y + preIndex * gap);
    Console_setColor(START_MENU_NOT_SELECT_FORE_COLOR, START_MENU_NOT_SELECT_BACK_COLOR);
    Sys_printf("%s", startMenuInfo[preIndex]);
    Console_gotoxy(startMenuSp.X, startMenuSp.Y + curIndex * gap);
    Console_setColor(START_MENU_SELECT_FORE_COLOR, START_MENU_SELECT_BACK_COLOR);
    Sys_printf("%s", startMenuInfo[curIndex]);
    Console_setColor(START_MENU_NOT_SELECT_FORE_COLOR, START_MENU_NOT_SELECT_BACK_COLOR);
}

void DM_drawRectangle(COORD sp, COORD size)
{
    /* draw vertex */
    //左上顶点
    Console_gotoxy(sp.X, sp.Y);
    Sys_printf(RECTANGLE_LEFT_TOP);
    //右上顶点
    Console_gotoxy(sp.X + size.X - 1, sp.Y);
    Sys_printf(RECTANGLE_RIGHT_TOP);
    //右下顶点
    Console_gotoxy(sp.X + size.X - 1, sp.Y + size.Y - 1);
    Sys_printf(RECTANGLE_RIGHT_BOTTOM);
    //左下顶点
    Console_gotoxy(sp.X, sp.Y + size.Y - 1);
    Sys_printf(RECTANGLE_LEFT_BOTTOM);

    //上横线
    Console_gotoxy(sp.X + 1, sp.Y);
    for (int i = 1; i < size.X - 1; i++)
    {
        Sys_printf(RECTANGLE_HORIZONTAL_LINE);
    }
    //下横线
    Console_gotoxy(sp.X + 1, sp.Y + size.Y - 1);
    for (int i = 1; i < size.X - 1; i++)
    {
        Sys_printf(RECTANGLE_HORIZONTAL_LINE);
    }

    //左竖线
    for (int i = 1; i < size.Y - 1; i++)
    {
        Console_gotoxy(sp.X, sp.Y + i);
        Sys_printf(RECTANGLE_VERTICAL_LINE);
    }

    //右竖线
    for (int i = 1; i < size.Y - 1; i++)
    {
        Console_gotoxy(sp.X + size.X - 1, sp.Y + i);
        Sys_printf(RECTANGLE_VERTICAL_LINE);
    }
}


void DM_clearRectangleBuff(COORD sp, COORD size)
{
    /* clear buff */
    for (int i = 0; i < size.Y; i++)
    {
        Console_gotoxy(sp.X, sp.Y + i);
        for (int j = 0; j < size.X; j++)
        {
            Sys_printf(EMPTY_LINE);
        }

    }
}
