#include "operator.h"

void OP_startMenu(opSts_t* event)
{
    if (kbhit())
    {
        char op = getch();
        switch (op)
        {
        case 'w':
        case 'W':
            event->pos.Y = event->pos.Y - 1;
            event->isPress = TRUE;
            break;
        case 's':
        case 'S':
            event->pos.Y = event->pos.Y + 1;
            event->isPress = TRUE;
            break;
        case 'j':
        case 'J':
            event->isComfirm = TRUE;
            event->isPress = TRUE;
            break;
        default:
            break;
        }
    }
}
