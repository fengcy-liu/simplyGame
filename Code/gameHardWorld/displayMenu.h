#pragma once
#ifndef DISPLAYMENU_H
#define DISPLAYMENU_H
#include "common_type.h"
#include "console.h"


#define START_MENU_LENGTH                        (4)
#define START_MENU_POSTION_OFFSET                (4)


void DM_initConsloe(void);
void DM_initCfg(void);
void DM_initStartMenu(void);
void DM_opStartMenu(int lastIndex, int curIndex);

#endif // !OPERATOR_H
