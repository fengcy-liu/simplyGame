#pragma once
#ifndef DISPLAYMENU_H
#define DISPLAYMENU_H
#include "common_type.h"
#include "console.h"
#include "public_cfg.h"


#define START_MENU_LENGTH                        (4)
#define START_MENU_POSTION_OFFSET                (4)

typedef enum enumMenuTag
{
    START_MENU=0,
    BATTLE_OPEATER_MENU=1
}enumMenu_t;

void DM_initConsloe(void);
void DM_initCfg(void);
void DM_initStartMenu(void);
void DM_updateStartMenu(int lastIndex, int curIndex);
void DM_drawRectangle(COORD sp, COORD size);
void DM_clearRectangleBuff(COORD sp, COORD size);


#endif // !OPERATOR_H
