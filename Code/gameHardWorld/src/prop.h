#pragma once
#ifndef PROP_H
#define PROP_H

#include "common_type.h"

#define PROP_MAX_NUM_OF_ALLPROP                            200u
#define PROP_MAX_NUM_OF_CUREPROP                           20u
#define PROP_MAX_NUM_OF_HEALPROP                           20u
#define PROP_MAX_NUM_OF_MULITYFUNCPROP                     20u


typedef enum enumEquipPartTag
{
    HEAD = 0,
    HAND,
    CLOTHES,
    SHOES,
    JEWELRY,
}enumEquipPart_t;

typedef struct equipTag
{
    u8_t id;
    char_t name;
    char_t desc;
    enumEquipPart_t part;
    s16_t addHp;
    s16_t addMp;
    s16_t addAttack;
    s16_t addDefense;
}equip_t;

typedef enum enumPropClassTag
{
    HEALPROP=0,
    CUREPROP,
    MULITYFUNCPROP,
    DAMAGEPROP,
}enumPropClass_t;

typedef enum enumCurePropClassTag
{
    CUREPETRIFACTION = 1,//petrifaction:石化
    CUREPARALYSIS,//paralysis:麻痹
    CUREINTOXICATION,//Intoxication:中毒
    CURELACERATE,//Lacerate:割裂
    CUREBLIND = 5,//blind:失明
    CUREALL,//所有异常状态
}enumCurePropClass_t;

typedef struct propListTag
{
    u8_t id;
    u8_t dstId;
    enumPropClass_t type;
}propList_t;

typedef struct healPropTag
{
    u8_t id;
    char_t name[MAX_LENGTH_OF_NAME];
    char_t desc[MAX_LENGTH_OF_DESC];
    s16_t aHp;
    s16_t aMp;
}healProp_t;

typedef struct curePropTag
{
    u8_t id;
    char_t name[MAX_LENGTH_OF_NAME];
    char_t desc[MAX_LENGTH_OF_DESC];
    enumCurePropClass_t effect;
}cureProp_t;

typedef struct mulityFuncPropTag
{
    u8_t id;
    char_t name[MAX_LENGTH_OF_NAME];
    char_t desc[MAX_LENGTH_OF_DESC];
    s16_t aHp;
    s16_t aMp;
    enumCurePropClass_t effect;
}mulityFuncProp_t;


extern propList_t propAll[PROP_MAX_NUM_OF_ALLPROP];
extern cureProp_t curePropList[PROP_MAX_NUM_OF_CUREPROP];
extern healProp_t healPropList[PROP_MAX_NUM_OF_CUREPROP];
extern mulityFuncProp_t mfPropList[PROP_MAX_NUM_OF_CUREPROP];

#endif // !PROP_H

