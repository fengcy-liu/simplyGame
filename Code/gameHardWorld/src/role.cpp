#include "role.h"

/* �쳣״̬ */
char_t faultStatusString[ROLE_MAX_NUM_TYPE_OF_FAULT_STATUS][HALF_MAX_BYTES_OF_CHAR] =
{
    L"����",
    L"ʯ��",
    L"���",
    L"�ж�",
    L"����",
    L"ʧ��"
};

/* ����״̬ */
char_t gainStatusString[ROLE_MAX_NUM_TYPE_OF_GAIN_STATUS][HALF_MAX_BYTES_OF_CHAR] =
{
    L"",//����
    L"����up",
    L"����up",
    L"����up",
    L" ",
    L" "
};

damageSkill_t damSkillList[ROLE_MAX_NUM_OF_DAMAGE_SKILLLIST] =
{
    /* ROLE */
    /* id;     name                      desc                                          maxObj   attackCntr   damage    damCoeff    extStsProp     prop        mana  */
    {   0,  L"ʮ����",      L"�Եз�����һ��ʮ����ն�����һ���˺�",                       1,        1,          200,        0   ,       0,       NONEFAULT,     75  },
    {   1,  L"������",      L"��������3�ι�����ÿ�ι����˺��������������˺�",              1,        3,            0,        0.75,       0,       NONEFAULT,     25  },
    /* ENEMY  */
    {   2,  L"��ײ"  ,      L"��ײ������С������Ե���",                                   1,        1,            0,        1.20,       0.1,     PARALYSIS,     25  },
    {   3,  L"���ݹ���",    L"�³����ж��ص����ݹ������ˣ��͸���ʹ�����ж�",               1,        1,            0,        0.80,       0.2,     INTOXICATION,  25  },
    {   4,  L"����ħ��",    L"����ĳ��Ƶ�ʵ�������ʹ���������˯��",                       1,        1,            0,        0,          1.0,     SLEEP,         25  },
    {   5,  L"��Ѫ",        L"˺ҧ���ˣ����ظ�����һ������ֵ",                             1,        1,            0,        1,          0.0,     NONEFAULT,     25  },
};

