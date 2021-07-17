#include "operator.h"
#include "displayBattle.h"
#include "displayMenu.h"

bool_t OP_startMenu(opSts_t* event)
{
    bool_t isPress = FALSE;
    if (kbhit())
    {
        char op = getch();
        switch (op)
        {
        case KEY_UP_LOWER:
        case KEY_UP_UPPER:
            event->pos.Y = event->pos.Y - 1;
            isPress = TRUE;
            break;
        case KEY_DOWN_LOWER:
        case KEY_DOWN_UPPER:
            event->pos.Y = event->pos.Y + 1;
            isPress = TRUE;
            break;
        case KEY_DONE_LOWER:
        case KEY_DONE_UPPER:
            event->isDone = TRUE;
            isPress = TRUE;
            break;
        default:
            break;
        }
        event->pos.Y = WAPPED(event->pos.Y, 0, (s8_t)START_MENU_LENGTH);
    }
    return isPress;
}

bool_t OP_battleOpMenu(opSts_t* event)
{
    bool_t isPress = FALSE;
    if (kbhit())
    {
        char op = getch();
        switch (op)
        {
        case KEY_UP_LOWER:
        case KEY_UP_UPPER:
            event->pos.Y = event->pos.Y - 1;
            isPress = TRUE;
            break;
        case KEY_DOWN_LOWER:
        case KEY_DOWN_UPPER:
            event->pos.Y = event->pos.Y + 1;
            isPress = TRUE;
            break;
        case KEY_DONE_LOWER:
        case KEY_DONE_UPPER:
            event->isDone = TRUE;
            isPress = TRUE;
            break;
        case KEY_CANCEL_LOWER:
        case KEY_CANCEL_UPPER:
            event->isCancel = TRUE;
            isPress = TRUE;
            break;
        default:
            break;
        }
        event->pos.Y = WAPPED(event->pos.Y, 0, (s8_t)BATTLE_BUTTON_NUM);
    }
    return isPress;
}

bool_t OP_battleAction(opSts_t* event, u8_t maxnum)
{
    bool_t isPress = FALSE;
    if (kbhit())
    {
        char op = getch();
        switch (op)
        {
        case KEY_UP_LOWER:
        case KEY_UP_UPPER:
            event->pos.Y = event->pos.Y - 1;
            isPress = TRUE;
            break;
        case KEY_DOWN_LOWER:
        case KEY_DOWN_UPPER:
            event->pos.Y = event->pos.Y + 1;
            isPress = TRUE;
            break;
        case KEY_DONE_LOWER:
        case KEY_DONE_UPPER:
            event->isDone = TRUE;
            isPress = TRUE;
            break;
        case KEY_CANCEL_LOWER:
        case KEY_CANCEL_UPPER:
            event->isCancel = TRUE;
            isPress = TRUE;
            break;
        default:
            break;
        }
        event->pos.Y = WAPPED(event->pos.Y, 0, (s8_t)maxnum);
    }
    return isPress;
}

bool_t OP_battleProp(opSts_t* event, u8_t maxnum)
{
    bool_t isPress = FALSE;
    if (kbhit())
    {
        char op = getch();
        switch (op)
        {
        case KEY_UP_LOWER:
        case KEY_UP_UPPER:
            event->pos.Y = event->pos.Y - 1;
            isPress = TRUE;
            break;
        case KEY_DOWN_LOWER:
        case KEY_DOWN_UPPER:
            event->pos.Y = event->pos.Y + 1;
            isPress = TRUE;
            break;
        case KEY_LEFT_LOWER:
        case KEY_LEFT_UPPER:
            event->pos.Y = event->pos.X - 1;
            isPress = TRUE;
            break; 
        case KEY_RIGHT_LOWER:
        case KEY_RIGHT_UPPER:
                event->pos.Y = event->pos.X + 1;
                isPress = TRUE;
                break;
        case KEY_DONE_LOWER:
        case KEY_DONE_UPPER:
            event->isDone = TRUE;
            isPress = TRUE;
            break;
        case KEY_CANCEL_LOWER:
        case KEY_CANCEL_UPPER:
            event->isCancel = TRUE;
            isPress = TRUE;
            break;
        default:
            break;
        }
        event->pos.X = MAX(event->pos.X, 0);
        event->pos.Y = MAX(event->pos.Y, 0);
    }
    return isPress;
}
