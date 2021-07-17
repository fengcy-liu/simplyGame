#pragma once
#ifndef OPERATOR_H
#define OPERATOR_H
#include "common_type.h"
#include "public_cfg.h"

#define KEY_UP_LOWER                            'w'
#define KEY_UP_UPPER                            'W'

#define KEY_DOWN_LOWER                          's'
#define KEY_DOWN_UPPER                          'S'

#define KEY_LEFT_LOWER                          'a'
#define KEY_LEFT_UPPER                          'A'

#define KEY_RIGHT_LOWER                         'd'
#define KEY_RIGHT_UPPER                         'D'

#define KEY_DONE_LOWER                          'j'
#define KEY_DONE_UPPER                          'J'

#define KEY_CANCEL_LOWER                        'k'
#define KEY_CANCEL_UPPER                        'K'



typedef struct opStsTag
{
    COORD pos;
    bool_t isDone;
    bool_t isCancel;
}opSts_t;

bool_t OP_startMenu(opSts_t* event);
bool_t OP_battleOpMenu(opSts_t* event);
bool_t OP_battleAction(opSts_t* event, u8_t maxnum);

#endif // !OPERATOR_H

