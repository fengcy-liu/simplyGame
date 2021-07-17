#pragma once
#ifndef ROLE_H
#define ROLE_H
#include "common_type.h"
#include "public_cfg.h"

#define ROLE_MAX_ROLE_NUM_OF_TEAM                   4u
#define ROLE_MAX_NUM_OF_ENEMY                       4u
#define ROLE_PART_NUM_OF_EQUIPMENT                  5u

#define ROLE_MAX_NUM_TYPE_OF_FAULT_STATUS           7u
#define ROLE_MAX_NUM_TYPE_OF_GAIN_STATUS            6u

#define ROLE_MAX_NUM_OF_FAULT_STATUS                6u
#define ROLE_MAX_NUM_OF_GAIN_STATUS                 6u

#define ROLE_MAX_SKILL_NUM_OF_OBJECT                8u

#define ROLE_MAX_NUM_SKILLLIST                      100u
#define ROLE_MAX_NUM_OF_DAMAGE_SKILLLIST            20u

#define ROLE_CRIT_COEFF                             2.0f

typedef enum enumTeamTypeTag
{
    ROLE = 0,
    ENEMY
}enumTeamType_t;

typedef enum enumRoleFaultStatusTag
{
    NONEFAULT = 0,
    PETRIFACTION = 1,//petrifaction:石化
    PARALYSIS,//paralysis:麻痹
    INTOXICATION,//Intoxication:中毒
    LACERATE,//Lacerate:割裂
    BLIND = 5,//blind:失明
    SLEEP //sleep:睡眠
}enumRoleFaultStatus_t;

typedef enum enumRoleGainStatusTag
{
    NONEGAIN = 0,
    ATTACK_UP = 1,//攻击力上升
    DEFENSE_UP, //防御力上升
    DODGE_UP, //闪避力上升
    REVERED1, //
    REVERED2 = 5, //
}enumRoleGainStatus_t;

typedef struct damageSkillTag
{
    u8_t id;
    char_t name[MAX_LENGTH_OF_NAME];//技能名字
    char_t desc[MAX_LENGTH_OF_DESC];//描述
    u8_t maxObj;
    u8_t attackCntr;
    s16_t damage;
    f32_t damCoeff;
    f32_t extStsPro;
    enumRoleFaultStatus_t extSts;
    s16_t costMp;
}damageSkill_t;

typedef struct healSkillTag
{
    u8_t id;
    char_t name[MAX_LENGTH_OF_NAME];//技能名字
    char_t desc[MAX_LENGTH_OF_DESC];//描述
    u8_t maxObj;
    u8_t attackCntr;
    s16_t damage;
    f32_t damCoeff;
    enumRoleGainStatus_t extSts;
    s16_t costMp;
}healSkill_t;

typedef struct baseAttrTag
{
    f32_t attackVal;//攻击力
    f32_t defenseVal;//防御力
    s16_t currentBlood;//当前血量
    s16_t maxBlood;//最大血量
    s16_t currentMana;//当前蓝量
    s16_t maxMana;//最大蓝量
    f32_t SPD;//行动力
    f32_t DEX;//命中率
    f32_t AGL;//灵敏度，决定闪避概率
    f32_t CRIT;//暴击率
}baseAttr_t;

typedef struct stsRecordTag
{
    u8_t bKeep : 1;
    u8_t cntr : 4;
    u8_t maxKeepCntr : 3;
}stsRecord_t;
typedef struct roleBuffTag
{
    /* 增益 */
    stsRecord_t attackup1;
    stsRecord_t defenceup1;
    stsRecord_t dodgeup1;
}roleBuff_t;

typedef struct roleDebuffTag
{
    /* 负面状态 */
    stsRecord_t petrifaction;
    stsRecord_t paralysis;
    stsRecord_t intoxication;
    stsRecord_t lacerate;
    stsRecord_t blind;
    stsRecord_t sleep;
}roleDebuff_t;

typedef struct roleTag
{
    u8_t id;
    u8_t careerId;
    u8_t quene;//序号
    u8_t level;//当前等级
    bool_t isValid;//是否存活
    u32_t exp;//当前经验值
    u32_t sumExp2nextLvl;//下一级所需经验 
    char_t name[MAX_BYTES_OF_CHAR];
    char_t gainSts[MAX_BYTES_OF_CHAR];
    char_t faultSts[MAX_BYTES_OF_CHAR];
    baseAttr_t attr;//基本属性，含装备
    roleBuff_t buff;
    roleDebuff_t debuff;

    u8_t skillIdNum;
    u8_t skillIdList[ROLE_MAX_SKILL_NUM_OF_OBJECT];

    u8_t equipIdList[ROLE_PART_NUM_OF_EQUIPMENT];
    /* for battle command */
    COORD battlePostion;//战斗位置
    baseAttr_t batAttr;//战斗属性
#ifndef _BIT_MASK_COMMAND
    void* actionCommand;
#else
    u32_t actionCommand;
#endif
}role_t;

typedef struct roleTeamTag
{
    u8_t num;
    COORD mapPos;
    struct roprop
    {
        u8_t propNum;
        u8_t list[200];
        u8_t keepCntr[200];
    } userProp;
    role_t role[ROLE_MAX_ROLE_NUM_OF_TEAM];
}roleTeam_t;

typedef struct enemyTag
{
    u8_t id;
    u8_t quene;//序号
    u8_t enemyClassId;
    bool_t isValid;//是否存活
    u32_t exp;//可获取经验值
    char_t name[MAX_BYTES_OF_CHAR];

    baseAttr_t attr;//基本属性
    baseAttr_t batAttr;//战斗属性
    roleBuff_t buff;
    roleDebuff_t debuff;
    char_t gainSts[MAX_BYTES_OF_CHAR];
    char_t faultSts[MAX_BYTES_OF_CHAR];
    /* for battle command */
    COORD battlePostion;
#ifndef _BIT_MASK_COMMAND
    void* actionCommand;
#else
    u32_t actionCommand;
#endif
}enemy_t;

typedef struct enemyTeamTag
{
    u8_t num;
    enemy_t enemy[ROLE_MAX_NUM_OF_ENEMY];
}enemyTeam_t;


extern damageSkill_t damSkillList[ROLE_MAX_NUM_OF_DAMAGE_SKILLLIST];
extern char_t faultStatusString[ROLE_MAX_NUM_TYPE_OF_FAULT_STATUS][HALF_MAX_BYTES_OF_CHAR];
extern char_t gainStatusString[ROLE_MAX_NUM_TYPE_OF_GAIN_STATUS][HALF_MAX_BYTES_OF_CHAR];

#endif // !ROLE_H
