#pragma once
#ifndef OPERATOR_H
#define OPERATOR_H
#include "common_type.h"

typedef struct opStsTag
{
	COORD pos;
	bool isPress;
	bool isComfirm;
}opSts_t;

void OP_startMenu(opSts_t *event);

#endif // !OPERATOR_H

