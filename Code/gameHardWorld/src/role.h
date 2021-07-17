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
    PETRIFACTION = 1,//petrifaction:ʯ��
    PARALYSIS,//paralysis:���
    INTOXICATION,//Intoxication:�ж�
    LACERATE,//Lacerate:����
    BLIND = 5,//blind:ʧ��
    SLEEP //sleep:˯��
}enumRoleFaultStatus_t;

typedef enum enumRoleGainStatusTag
{
    NONEGAIN = 0,
    ATTACK_UP = 1,//����������
    DEFENSE_UP, //����������
    DODGE_UP, //����������
    REVERED1, //
    REVERED2 = 5, //
}enumRoleGainStatus_t;

typedef struct damageSkillTag
{
    u8_t id;
    char_t name[MAX_LENGTH_OF_NAME];//��������
    char_t desc[MAX_LENGTH_OF_DESC];//����
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
    char_t name[MAX_LENGTH_OF_NAME];//��������
    char_t desc[MAX_LENGTH_OF_DESC];//����
    u8_t maxObj;
    u8_t attackCntr;
    s16_t damage;
    f32_t damCoeff;
    enumRoleGainStatus_t extSts;
    s16_t costMp;
}healSkill_t;

typedef struct baseAttrTag
{
    f32_t attackVal;//������
    f32_t defenseVal;//������
    s16_t currentBlood;//��ǰѪ��
    s16_t maxBlood;//���Ѫ��
    s16_t currentMana;//��ǰ����
    s16_t maxMana;//�������
    f32_t SPD;//�ж���
    f32_t DEX;//������
    f32_t AGL;//�����ȣ��������ܸ���
    f32_t CRIT;//������
}baseAttr_t;

typedef struct stsRecordTag
{
    u8_t bKeep : 1;
    u8_t cntr : 4;
    u8_t maxKeepCntr : 3;
}stsRecord_t;
typedef struct roleBuffTag
{
    /* ���� */
    stsRecord_t attackup1;
    stsRecord_t defenceup1;
    stsRecord_t dodgeup1;
}roleBuff_t;

typedef struct roleDebuffTag
{
    /* ����״̬ */
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
    u8_t quene;//���
    u8_t level;//��ǰ�ȼ�
    bool_t isValid;//�Ƿ���
    u32_t exp;//��ǰ����ֵ
    u32_t sumExp2nextLvl;//��һ�����辭�� 
    char_t name[MAX_BYTES_OF_CHAR];
    char_t gainSts[MAX_BYTES_OF_CHAR];
    char_t faultSts[MAX_BYTES_OF_CHAR];
    baseAttr_t attr;//�������ԣ���װ��
    roleBuff_t buff;
    roleDebuff_t debuff;

    u8_t skillIdNum;
    u8_t skillIdList[ROLE_MAX_SKILL_NUM_OF_OBJECT];

    u8_t equipIdList[ROLE_PART_NUM_OF_EQUIPMENT];
    /* for battle command */
    COORD battlePostion;//ս��λ��
    baseAttr_t batAttr;//ս������
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
    u8_t quene;//���
    u8_t enemyClassId;
    bool_t isValid;//�Ƿ���
    u32_t exp;//�ɻ�ȡ����ֵ
    char_t name[MAX_BYTES_OF_CHAR];

    baseAttr_t attr;//��������
    baseAttr_t batAttr;//ս������
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
