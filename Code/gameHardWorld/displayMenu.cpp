#include "displayMenu.h"

#define START_MENU_SELECT_FORE_COLOR                   FORE_WHITE   
#define START_MENU_SELECT_BACK_COLOR				   BACK_RED   
#define START_MENU_NOT_SELECT_FORE_COLOR               FORE_WHITE   
#define START_MENU_NOT_SELECT_BACK_COLOR			   FORE_DARK     



const char_t startMenuInfo[START_MENU_LENGTH][20] =
{
	L"开始冒险",
	L"加载记录",
	L"设    置",
	L"退    出"
};

COORD startPos = { 56, 7 };
u8_t gap = 2;

COORD fontSize = { 18, 25 };
COORD buffSize = { 120,40 };

void DM_initConsloe(void)
{

	Console_setConsoleCode(CODE_UNICODE);
	Console_setTitle((char_t *)L"GAME");
	Console_setFont(fontSize.X, fontSize.Y, 0);
	COORD X;
	Console_SetBuffSize(buffSize);
	Console_hideCursor();
	Console_getBuffSize(&X);
	//Sys_printf(L"%d, %d\n", X.X, X.Y);
}

void DM_initCfg(void)
{
#if 0
	COORD winSize;
	Console_getBuffSize(&winSize);
	startPos.X = winSize.X / 2 - START_MENU_POSTION_OFFSET;
	startPos.Y = 5;
#endif
}

void DM_initStartMenu()
{
	int i;
	Console_gotoxy(startPos.X, startPos.Y);
	Console_setColor(START_MENU_SELECT_FORE_COLOR, START_MENU_SELECT_BACK_COLOR);
	Sys_printf(L"%s", startMenuInfo[0]);
	Console_setColor(START_MENU_NOT_SELECT_FORE_COLOR, START_MENU_NOT_SELECT_BACK_COLOR);

	for ( i = 1; i < START_MENU_LENGTH; i++)
	{
		Console_gotoxy(startPos.X, startPos.Y + i * gap);
		Sys_printf(L"%s", startMenuInfo[i]);
	}

}

void DM_opStartMenu(int preIndex, int curIndex)
{
	Console_gotoxy(startPos.X, startPos.Y + preIndex * gap);
	Console_setColor(START_MENU_NOT_SELECT_FORE_COLOR, START_MENU_NOT_SELECT_BACK_COLOR);
	Sys_printf(L"%s", startMenuInfo[preIndex]);
	Console_gotoxy(startPos.X, startPos.Y + curIndex * gap);
	Console_setColor(START_MENU_SELECT_FORE_COLOR, START_MENU_SELECT_BACK_COLOR);
	Sys_printf(L"%s", startMenuInfo[curIndex]);
	Console_setColor(START_MENU_NOT_SELECT_FORE_COLOR, START_MENU_NOT_SELECT_BACK_COLOR);
}
