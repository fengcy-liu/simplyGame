#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include "common_type.h"

/* console color */
#define FORE_WHITE                          FOREGROUND_RED|FOREGROUND_BLUE| FOREGROUND_GREEN
#define BACK_WHITE                          BACKGROUND_RED|BACKGROUND_BLUE| BACKGROUND_GREEN
#define FORE_DARK                           0
#define BACK_DARK                           0

#define FORE_BLUE                           FOREGROUND_BLUE           /*蓝色文本属性  */
#define FORE_GREEN                          FOREGROUND_GREEN          /*绿色文本属性  */
#define FORE_RED                            FOREGROUND_RED            /*红色文本属性  */
#define FORE_PURPLE                         FORE_BLUE | FORE_RED      /*紫色文本属性  */
#define FORE_CYAN                           FORE_BLUE | FORE_GREEN    /*青色文本属性  */
#define FORE_YELLOW                         FORE_RED | FORE_GREEN     /*黄色文本属性  */
#define FORE_GRAY                           FOREGROUND_INTENSITY      /*灰色文本属性  */
#define BACK_BLUE                           BACKGROUND_BLUE           /*蓝色背景属性  */
#define BACK_GREEN                          BACKGROUND_GREEN          /*绿色背景属性  */
#define BACK_RED                            BACKGROUND_RED            /*绿色背景属性  */
#define BACK_PURPLE                         BACK_BLUE | BACK_RED      /*紫色背景属性  */
#define BACK_CYAN                           BACK_BLUE | BACK_GREEN    /*青色背景属性  */
#define BACK_YELLOW                         BACK_RED | BACK_GREEN     /*黄色背景属性  */
#define BACK_GRAY                           BACKGROUND_INTENSITY      /*灰色背景属性  */

/* console code */
#define CODE_GBK                            936
#define CODE_UNICODE                        65001


#ifdef _UNICODE
#define Sys_printf                          wprintf
#else
#define Sys_printf                          printf
#endif

void Console_getWinSize(COORD* winSize);
void Console_setConsoleCode(s32_t code);
void Console_getBuffSize(COORD* buffSize);
void Console_SetBuffSize(COORD buffSize);
void Console_gotoxy(int posx, int posy);
void Console_hideCursor(void);
void Console_setColor(int foreColor, int backColor);
void Console_setFont(int width, int height, int fontWidth);
void Console_setTitle(char_t* title);

#endif
