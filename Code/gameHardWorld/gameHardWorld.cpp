// gameHardWorld.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "console.h"
#include "displayMenu.h"
#include "operator.h"
#include "gameHardWorld.h"


void mainGame(void)
{
    
    DM_initConsloe();
    DM_initCfg();
    DM_initStartMenu();
    while (TRUE)
    {
        static opSts_t event;
        static int lastPosY;
        lastPosY = event.pos.Y;
        OP_startMenu(&event);
        event.pos.Y = WAIP(event.pos.Y, 0 , (s16_t)START_MENU_LENGTH);
        Console_gotoxy(0,7);
        printf("pos : %d", event.pos.Y);
        if (event.pos.Y < 0 && event.pos.Y >= START_MENU_LENGTH)
        {
            Console_gotoxy(0, 40);
            printf("error : %d \n", event.pos.Y);
        }
        if (event.isComfirm == TRUE)
        {
            system("cls");
            printf("start game\n");
            break;
        }
        if (event.isPress == TRUE)
        {
            event.isPress = FALSE;
            DM_opStartMenu(lastPosY, event.pos.Y);
        }
    }
}

