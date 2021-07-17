#include "role.h"

/* 异常状态 */
char_t faultStatusString[ROLE_MAX_NUM_TYPE_OF_FAULT_STATUS][HALF_MAX_BYTES_OF_CHAR] =
{
    L"正常",
    L"石化",
    L"麻痹",
    L"中毒",
    L"割裂",
    L"失明"
};

/* 增益状态 */
char_t gainStatusString[ROLE_MAX_NUM_TYPE_OF_GAIN_STATUS][HALF_MAX_BYTES_OF_CHAR] =
{
    L"",//正常
    L"攻击up",
    L"防御up",
    L"闪避up",
    L" ",
    L" "
};

damageSkill_t damSkillList[ROLE_MAX_NUM_OF_DAMAGE_SKILLLIST] =
{
    /* ROLE */
    /* id;     name                      desc                                          maxObj   attackCntr   damage    damCoeff    extStsProp     prop        mana  */
    {   0,  L"十字劈",      L"对敌方发动一次十字劈斩，造成一定伤害",                       1,        1,          200,        0   ,       0,       NONEFAULT,     75  },
    {   1,  L"三连击",      L"连续发动3次攻击，每次攻击伤害低于正常攻击伤害",              1,        3,            0,        0.75,       0,       NONEFAULT,     25  },
    /* ENEMY  */
    {   2,  L"冲撞"  ,      L"冲撞攻击，小概率麻痹敌人",                                   1,        1,            0,        1.20,       0.1,     PARALYSIS,     25  },
    {   3,  L"泡泡攻击",    L"吐出含有毒素的泡泡攻击敌人，低概率使敌人中毒",               1,        1,            0,        0.80,       0.2,     INTOXICATION,  25  },
    {   4,  L"催眠魔音",    L"发出某种频率的音波，使敌人陷入沉睡中",                       1,        1,            0,        0,          1.0,     SLEEP,         25  },
    {   5,  L"吸血",        L"撕咬敌人，并回复自身一定生命值",                             1,        1,            0,        1,          0.0,     NONEFAULT,     25  },
};

