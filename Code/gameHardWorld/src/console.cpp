
#include "console.h"

/*
1，控制台初始化
    * GetConsoleScreenBufferInfo 获取控制台窗口信息

    * GetConsoleTitle 获取控制台窗口标题

    * ScrollConsoleScreenBuffer 在缓冲区中移动数据块

    * SetConsoleScreenBufferSize 更改指定缓冲区大小

    * SetConsoleTitleA 设置控制台窗口标题

    * SetConsoleWindowInfo 设置控制台窗口信息

    * GetStdHandle 获取句柄

    * CloseHandle(hOut); 关闭标准输出设备句柄
2，设置文本属性
    * SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof cfi;
        cfi.nFont = 0;
        cfi.dwFontSize.X = 0; //字宽
        cfi.dwFontSize.Y = 20;//字高
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;//粗细
        wcscpy_s(cfi.FaceName, L"Raster"); //设置字体，此处设为点阵字体
        cbSize
        The size of this structure, in bytes. This member must be set to sizeof(CONSOLE_FONT_INFOEX) before calling GetCurrentConsoleFontEx or it will fail.

        nFont
        The index of the font in the system’s console font table.

        dwFontSize
        A COORD structure that contains the width and height of each character in the font, in logical units. The X member contains the width, while the Y member contains the height.

        FontFamily
        The font pitch and family. For information about the possible values for this member, see the description of the tmPitchAndFamily member of the TEXTMETRIC structure.

        FontWeight
        The font weight. The weight can range from 100 to 1000, in multiples of 100. For example, the normal weight is 400, while 700 is bold.

        FaceName
        The name of the typeface (such as Courier or Arial).
    * BOOL SetConsoleTextAttribute(HANDLE hConsoleOutput, WORD wAttributes);//句柄, 文本属性
        WORD文本属性预定义宏：(可以直接用16进制表示，WORD w = 0xf0;前一位表示背景色，后一位代表前景色)
        FOREGROUND_BLUE 蓝色
        FOREGROUND_GREEN 绿色
        FOREGROUND_RED 红色
        FOREGROUND_INTENSITY 加强
        BACKGROUND_BLUE 蓝色背景
        BACKGROUND_GREEN 绿色背景
        BACKGROUND_RED 红色背景
        BACKGROUND_INTENSITY 背景色加强
        COMMON_LVB_REVERSE_VIDEO 反色
        当前文本属性信息可通过调用函数GetConsoleScreenBufferInfo后，在CONSOLE_SCREEN_ BUFFER_INFO结构成员wAttributes中得到。

    在指定位置处写属性
    * BOOL WriteConsoleOutputAttribute(HANDLE hConsoleOutput,  CONST WORD *lpAttribute, DWORD nLength,
                                        COORD dwWriteCoord, LPDWORD lpNumberOfAttrsWritten);
                                        //句柄, 属性, 个数, 起始位置, 已写个数
    
    移动文本位置位置
    BOOL ScrollConsoleScreenBuffer(HANDLE hConsoleOutput, CONST SMALL_RECT* lpScrollRectangle, CONST SMALL_RECT* lpClipRectangle,
                                   COORD dwDestinationOrigin,CONST CHAR_INFO* lpFill);
                                  // 句柄// 裁剪区域// 目标区域 // 新的位置// 填充字符

    填充指定数据的字符
    * BOOL FillConsoleOutputCharacter(HANDLE hConsoleOutput, TCHAR cCharacter, DWORD nLength, COORD dwWriteCoord,
        LPDWORD lpNumberOfCharsWritten);
    // 句柄， 字符， 字符个数， 起始位置， 已写个数

    在当前光标位置处插入指定数量的字符
    * BOOL WriteConsole(HANDLE hConsoleOutput, CONST VOID* lpBuffer, DWORD nNumberOfCharsToWrite,
        LPDWORD lpNumberOfCharsWritten, LPVOID lpReserved);
    //句柄, 字符串, 字符个数, 已写个数, 保留

    向指定区域写带属性的字符
    * BOOL WriteConsoleOutput(HANDLE hConsoleOutput, CONST CHAR_INFO* lpBuffer, COORD dwBufferSize,
        COORD dwBufferCoord, PSMALL_RECT lpWriteRegion);
    // 句柄 // 字符数据区// 数据区大小// 起始坐标// 要写的区域

    在指定位置处插入指定数量的字符
    * BOOL WriteConsoleOutputCharacter(HANDLE hConsoleOutput, LPCTSTR lpCharacter, DWORD nLength,
        COORD dwWriteCoord, LPDWORD lpNumberOfCharsWritten);
    // 句柄// 字符串// 字符个数// 起始位置// 已写个数

    填充字符属性
    * BOOL FillConsoleOutputAttribute(HANDLE hConsoleOutput, WORD wAttribute, DWORD nLength,
        COORD dwWriteCoord, LPDWORD lpNumberOfAttrsWritten);
    //句柄, 文本属性, 个数, 开始位置, 返回填充的个数

   设置代码页，
    * SetConsoleOutputCP(437);如（简体中文） 设置成936

3，光标操作控制
    设置光标位置
    * SetConsoleCursorPosition(HANDLE hConsoleOutput,COORD dwCursorPosition);
    设置光标信息
    * BOOL SetConsoleCursorInfo(HANDLE hConsoleOutput, PCONST PCONSOLE_CURSOR_INFO lpConsoleCursorInfo);
    获取光标信息
    * BOOL GetConsoleCursorInfo(HANDLE hConsoleOutput,  PCONSOLE_CURSOR_INFO lpConsoleCursorInfo);
    参数1：句柄；参数2：CONSOLE_CURSOR_INFO结构体{DWORD dwSize;(光标大小取值1-100)BOOL bVisible;（是否可见)}


*/

void Console_getWinSize(COORD* winSize)
{
    CONSOLE_SCREEN_BUFFER_INFO buffInfo;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(handle, &buffInfo);
    winSize->X = buffInfo.srWindow.Right - buffInfo.srWindow.Left;//宽
    winSize->Y = buffInfo.srWindow.Bottom - buffInfo.srWindow.Top;//高
}

void Console_setConsoleCode(s32_t code)
{
    char str[20];
    switch (code)
    {
    case CODE_GBK:
        setlocale(LC_ALL, "chs");//chs:简体中文，zh_CN.UTF-8 : utf-8
        break;
    case CODE_UNICODE:
        setlocale(LC_ALL, "zh_CN.UTF-8");//chs:简体中文，zh_CN.UTF-8 : utf-8
        break;
    default:
        break;
    }
    sprintf(str, "chcp %u && cls\n",code);
    system(str);
}

void Console_getBuffSize(COORD *buffSize)
{
    CONSOLE_SCREEN_BUFFER_INFO buffInfo;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(handle, &buffInfo);
    *buffSize = buffInfo.dwSize;
}

void Console_SetBuffSize(COORD buffSize)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT rc = { 1,1, buffSize.X, buffSize.Y };
    /* 必须先设置窗口大小，才能修改缓冲区大小 */
    SetConsoleWindowInfo(handle, TRUE, &rc);
    SetConsoleScreenBufferSize(handle, buffSize);
    system("cls");
}

void Console_gotoxy(int posx, int posy)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = posx;
    pos.Y = posy;
    SetConsoleCursorPosition(handle, pos);
}

void Console_hideCursor(void)
{
    CONSOLE_CURSOR_INFO cursor_info = { 1, 0 }; //第二个值为0表示隐藏光标
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void Console_setColor(int foreColor, int backColor)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    /*        
        FOREGROUND_BLUE 蓝色
        FOREGROUND_GREEN 绿色
        FOREGROUND_RED 红色
        FOREGROUND_INTENSITY 加强
        BACKGROUND_BLUE 蓝色背景
        BACKGROUND_GREEN 绿色背景
        BACKGROUND_RED 红色背景
        BACKGROUND_INTENSITY 背景色加强
        COMMON_LVB_REVERSE_VIDEO 反色
     */
    WORD fbColor = backColor | foreColor;
    SetConsoleTextAttribute(handle, fbColor);
}

void Console_setFont(int width, int height, int fontWidth)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    GetCurrentConsoleFontEx(handle, FALSE, &cfi);
    //cfi.nFont = 0;
    cfi.dwFontSize.X = width; //字宽
    cfi.dwFontSize.Y = height;//字高
    cfi.FontFamily = FF_SCRIPT;
    cfi.FontWeight = FW_NORMAL;//粗细
    //wcscpy_s(cfi.FaceName, L"Raster");
    SetCurrentConsoleFontEx(handle, FALSE, &cfi);
}

void Console_setTitle(char_t *title)
{
    SetConsoleTitle(title);
}

void Console_clearAll(void)
{
    system("cls");
}
