#include "displayBattle.h"
#include "prop.h"

#define BATTLE_OPEATER_MENU_SELECT_FORE_COLOR                   FORE_WHITE   
#define BATTLE_OPEATER_MENU_SELECT_BACK_COLOR				    BACK_RED   
#define BATTLE_OPEATER_MENU_NOT_SELECT_FORE_COLOR               FORE_WHITE   
#define BATTLE_OPEATER_MENU_NOT_SELECT_BACK_COLOR			    FORE_DARK     

#define BIT_OFFSET_ROLE_HP               9
#define BIT_OFFSET_ROLE_HP_START         5

u8_t DB_berserker(role_t* prole, roleTeam_t* rt, enemyTeam_t* et);
void DB_updateBatAttr(roleTeam_t* rt, enemyTeam_t* et);
void DB_calBatAttr(role_t* prole);
void DB_mulityFuncProp(char_t* msg, role_t* dstprole, mulityFuncProp_t* prop);
void DB_healProp(char_t* msg, role_t* dstprole, healProp_t* prop);
void DB_cureProp(char_t* msg, role_t* dstprole, cureProp_t* prop);
void DB_damageSkill(char_t* msg, damageSkill_t* skill, role_t* prole, enemy_t* penemy);


COORD DB_screenSize = {100, 40};//界面整体大小


COORD DB_battleAreaSp = { 10,5 };//战斗画面区起始位置
COORD DB_battleAreaSize = { 70,22 };//战斗画面区大小

COORD DB_opMenuSp = { 10 ,DB_battleAreaSp.Y + DB_battleAreaSize.Y + 1 };//操作栏起始位置
COORD DB_opMenuSize = { 10,9 };//操作栏大小

COORD DB_battleBuffSp = { DB_opMenuSp.X + DB_opMenuSize.X + 1 ,DB_opMenuSp.Y };//战斗信息缓冲区起始位置
COORD DB_battleBuffSize = { 40,9 };//战斗信息缓冲区大小

COORD DB_roleNameContextSp = { DB_battleBuffSp.X + 7 ,DB_battleBuffSp.Y - 3 };//回合角色指示框起始位置
COORD DB_roleNameContextSize = { 20,3 };//回合角色指示框大小

const COORD roleDisplaySize = { 14, 5 };


spd_t actionList[ROLE_MAX_NUM_OF_ENEMY + ROLE_MAX_ROLE_NUM_OF_TEAM];

char_t battleButton[BATTLE_BUTTON_NUM][HALF_MAX_BYTES_OF_CHAR] =
{
    L"攻 击",
    L"防 御",
    L"技 能",
    L"物 品",
    L"逃 跑",
};

void DB_initBattleScreen(roleTeam_t* rt, enemyTeam_t* et)
{

    Console_gotoxy(DB_battleAreaSp.X, 1);
    for (int i = 0; i < DB_battleAreaSize.X; i++)
    {
        Sys_printf("*");
    }
    Console_gotoxy(DB_battleAreaSp.X + DB_battleAreaSize.X/2 - 4, 1);
    Sys_printf("开始战斗");

    /* 初始化角色和敌人队伍的起始地址*/
    DB_drawRoleTeam(rt);
    DB_drawEnemyTeam(et);
    /* 画出对应的矩形框 */
    DM_drawRectangle(DB_opMenuSp, DB_opMenuSize);
    DM_drawRectangle(DB_battleBuffSp, DB_battleBuffSize);

    /* 画出操作栏 */
    COORD opSp;
    opSp.X = DB_opMenuSp.X + (DB_opMenuSize.X - 4) / 2;
    opSp.Y = DB_opMenuSp.Y + 2;

    Console_gotoxy(opSp.X, opSp.Y);
    Console_setColor(BATTLE_OPEATER_MENU_SELECT_FORE_COLOR, BATTLE_OPEATER_MENU_SELECT_BACK_COLOR);
    Sys_printf("%s", battleButton[0]);
    Console_setColor(BATTLE_OPEATER_MENU_NOT_SELECT_FORE_COLOR, BATTLE_OPEATER_MENU_NOT_SELECT_BACK_COLOR);

    for (int i = 1; i < BATTLE_BUTTON_NUM; i++)
    {
        Console_gotoxy(opSp.X, opSp.Y + i);
        Sys_printf("%s", battleButton[i]);
    }
}

void DB_updateBattleOpMenu(int preIndex, int curIndex)
{
    COORD opSp;
    opSp.X = DB_opMenuSp.X + (DB_opMenuSize.X - 4) / 2;
    opSp.Y = DB_opMenuSp.Y + 2;
    Console_gotoxy(opSp.X, opSp.Y + preIndex);
    Console_setColor(BATTLE_OPEATER_MENU_NOT_SELECT_FORE_COLOR, BATTLE_OPEATER_MENU_NOT_SELECT_BACK_COLOR);
    Sys_printf("%s", battleButton[preIndex]);
    Console_gotoxy(opSp.X, opSp.Y + curIndex);
    Console_setColor(BATTLE_OPEATER_MENU_SELECT_FORE_COLOR, BATTLE_OPEATER_MENU_SELECT_BACK_COLOR);
    Sys_printf("%s", battleButton[curIndex]);
    Console_setColor(BATTLE_OPEATER_MENU_NOT_SELECT_FORE_COLOR, BATTLE_OPEATER_MENU_NOT_SELECT_BACK_COLOR);
}

void DB_initBattleSkillMenu()
{
    DB_gotoBattleBuff(0);
}

u8_t DB_getNumBit(s16_t num)
{
    u8_t cntr = 1;
    while (num >= 10)
    {
        cntr++;
        num = num / 10;
    }
    return cntr;
}

void DB_updateRole(role_t* prole)
{
    u8_t bitpos;
    COORD temp;
    temp.X = prole->battlePostion.X;
    temp.Y = prole->battlePostion.Y - 1;

    Console_gotoxy(temp.X, temp.Y);
    Sys_printf("%s", prole->gainSts);

    Console_gotoxy(temp.X, temp.Y + 1);
    Sys_printf("%s", prole->name);

    bitpos = DB_getNumBit(prole->attr.currentBlood);
    Console_gotoxy(temp.X + BIT_OFFSET_ROLE_HP_START, temp.Y + 2);
    Sys_printf("    ");
    Console_gotoxy(temp.X + BIT_OFFSET_ROLE_HP - bitpos, temp.Y + 2);
    Sys_printf("%d", prole->attr.currentBlood);

    bitpos = DB_getNumBit(prole->attr.currentMana);
    Console_gotoxy(temp.X + BIT_OFFSET_ROLE_HP_START, temp.Y + 3);
    Sys_printf("    ");
    Console_gotoxy(temp.X + BIT_OFFSET_ROLE_HP - bitpos, temp.Y + 3);
    Sys_printf("%d", prole->attr.currentMana);

    Console_gotoxy(temp.X, temp.Y + 4);
    Sys_printf("状态:%s", prole->faultSts);
}

void DB_updateEnemy(enemy_t* penemy)
{
    u8_t bitpos;
    COORD temp;
    temp.X = penemy->battlePostion.X;
    temp.Y = penemy->battlePostion.Y - 1;

    Console_gotoxy(temp.X, temp.Y);
    Sys_printf("%s", penemy->gainSts);

    Console_gotoxy(temp.X, temp.Y + 1);
    Sys_printf("%s", penemy->name);

    bitpos = DB_getNumBit(penemy->attr.currentBlood);
    Console_gotoxy(temp.X + BIT_OFFSET_ROLE_HP_START, temp.Y + 2);
    Sys_printf("    ");
    Console_gotoxy(temp.X + BIT_OFFSET_ROLE_HP - bitpos, temp.Y + 2);
    Sys_printf("%d", penemy->attr.currentBlood);

    bitpos = DB_getNumBit(penemy->attr.currentMana);
    Console_gotoxy(temp.X + BIT_OFFSET_ROLE_HP_START, temp.Y + 3);
    Sys_printf("    ");
    Console_gotoxy(temp.X + BIT_OFFSET_ROLE_HP - bitpos, temp.Y + 3);
    Sys_printf("%d", penemy->attr.currentMana);

    Console_gotoxy(temp.X, temp.Y + 4);
    Sys_printf("状态:%s", penemy->faultSts);
}

void DB_drawRoleTeam(roleTeam_t* rt)
{
    COORD rtSp;
    u8_t bitpos;
    rtSp.X = DB_battleAreaSp.X;
    rtSp.Y = DB_battleAreaSp.Y + (DB_battleAreaSize.Y - rt->num * roleDisplaySize.Y) / 2;
    /* 画出我方队伍阵列 */
    for (int i = 0; i < rt->num; i++)
    {
        COORD temp;
        temp.X = rtSp.X + 1;
        temp.Y = rtSp.Y + rt->role[i].quene * roleDisplaySize.Y;

        Console_gotoxy(temp.X, temp.Y);
        Sys_printf("%s", rt->role[i].gainSts);

        Console_gotoxy(temp.X, temp.Y + 1);
        rt->role[i].battlePostion = { temp.X, temp.Y + 1 };
        Sys_printf("%s", rt->role[i].name);

        Console_gotoxy(temp.X, temp.Y + 2);
        Sys_printf(" HP :    /%d", rt->role[i].attr.maxBlood);
        bitpos = DB_getNumBit(rt->role[i].attr.currentBlood);
        Console_gotoxy(temp.X + BIT_OFFSET_ROLE_HP - bitpos, temp.Y + 2);
        Sys_printf("%d", rt->role[i].attr.currentBlood);

        Console_gotoxy(temp.X, temp.Y + 3);
        Sys_printf(" MP :    /%d", rt->role[i].attr.maxMana);
        bitpos = DB_getNumBit(rt->role[i].attr.currentMana);
        Console_gotoxy(temp.X + BIT_OFFSET_ROLE_HP - bitpos, temp.Y + 3);
        Sys_printf("%d", rt->role[i].attr.currentMana);

        Console_gotoxy(temp.X, temp.Y + 4);
        Sys_printf("状态:%s", rt->role[i].faultSts);
    }
}

void DB_drawEnemyTeam(enemyTeam_t* et)
{
    COORD etSp;
    u8_t bitpos;
    etSp.X = DB_battleAreaSp.X + DB_battleAreaSize.X - roleDisplaySize.X;
    etSp.Y = DB_battleAreaSp.Y + (DB_battleAreaSize.Y - et->num * roleDisplaySize.Y) / 2;

    /* 画出敌方队伍阵列 */
    for (int i = 0; i < et->num; i++)
    {
        COORD temp;
        temp.X = etSp.X + 1;
        temp.Y = etSp.Y + et->enemy[i].quene * roleDisplaySize.Y;

        Console_gotoxy(temp.X, temp.Y);
        Sys_printf("%s", et->enemy[i].gainSts);

        Console_gotoxy(temp.X, temp.Y + 1);
        et->enemy[i].battlePostion = { temp.X, temp.Y + 1 };
        Sys_printf("%s", et->enemy[i].name);

        Console_gotoxy(temp.X, temp.Y + 2);
        Sys_printf(" HP :    /%d", et->enemy[i].attr.currentBlood, et->enemy[i].attr.maxBlood);
        bitpos = DB_getNumBit(et->enemy[i].attr.currentBlood);
        Console_gotoxy(temp.X + BIT_OFFSET_ROLE_HP - bitpos, temp.Y + 2);
        Sys_printf("%d", et->enemy[i].attr.currentBlood);

        Console_gotoxy(temp.X, temp.Y + 3);
        Sys_printf(" MP :    /%d",et->enemy[i].attr.maxMana);
        bitpos = DB_getNumBit(et->enemy[i].attr.currentMana);
        Console_gotoxy(temp.X + BIT_OFFSET_ROLE_HP - bitpos, temp.Y + 3);
        Sys_printf("%d", et->enemy[i].attr.maxMana);

        Console_gotoxy(temp.X, temp.Y + 4); 
        Sys_printf("状态:");
        Sys_printf("%s", et->enemy[i].faultSts);
    }
}


void DB_drawRoleNameContext(char_t *roleName)
{
    COORD sp,size;
    sp = DB_roleNameContextSp;
    size = DB_roleNameContextSize;
    DM_drawRectangle(sp, size);
    Console_gotoxy(sp.X + 2, sp.Y + 1);
    Sys_printf("%s",roleName);
}

void DB_clearRoleNameContext(void)
{
    COORD sp, size;
    sp = DB_roleNameContextSp;
    size = DB_roleNameContextSize;
    DM_clearRectangleBuff(sp, size);
}

u8_t DB_getValidRole(roleTeam_t* rt)
{
    u8_t cntr = 0;
    for (int i = 0; i < rt->num; i++)
    {
        if (rt->role[i].isValid == TRUE)
        {
            cntr++;
        }
    }
    return cntr;
}

u8_t DB_getValidEnemy(enemyTeam_t* et)
{
    u8_t cntr = 0;
    for (int i = 0; i < et->num; i++)
    {
        if (et->enemy[i].isValid == TRUE)
        {
            cntr++;
        }
    }
    return cntr;
}

u8_t DB_getMappedRoleId(roleTeam_t* rt, u8_t index)
{
    u8_t mappedId = 0;
    for (int i = 0; i < rt->num; i++)
    {
        if (rt->role[i].isValid == TRUE)
        {
            if (mappedId == index)
            {
                mappedId = i;
                break;
            }
            mappedId++;
            
        }
    }
    return mappedId;
}

u8_t DB_getMappedEnemyId(enemyTeam_t* et, u8_t index)
{
    u8_t mappedId = 0;
    for (int i = 0; i < et->num; i++)
    {
        if (et->enemy[i].isValid == TRUE)
        {
            if (mappedId == index)
            {
                mappedId = i;
                break;
            }
            mappedId++;

        }
    }
    return mappedId;
}

void DB_writeBattleBuff(char_t *str, bool_t bClean)
{
    static int index;
    if ((index == DB_battleBuffSize.Y - 2)|| (bClean == TRUE))
    {
        DB_clearBattleBuff();
        index = 0;
    }
    Console_gotoxy(DB_battleBuffSp.X + 3, DB_battleBuffSp.Y + 1 + index);
    Sys_printf("%s", str);
    index++;
}

void DB_gotoBattleBuff(u8_t index)
{
    /* first clear buff */
    index = index % (DB_battleBuffSize.Y - 2);
    if (index == 0)
    {
        DB_clearBattleBuff();
    }
    Console_gotoxy(DB_battleBuffSp.X + 3, DB_battleBuffSp.Y + 1 + index);
}

void DB_clearBattleBuff(void)
{
    COORD sp = { DB_battleBuffSp.X + 1, DB_battleBuffSp.Y + 1 };
    COORD size = { DB_battleBuffSize.X - 2 , DB_battleBuffSize.Y - 2 };
    DM_clearRectangleBuff(sp,size);
}

void DB_sortSpd(spd_t *pspdList, roleTeam_t* rt, enemyTeam_t* et)
{
    u8_t sum = 0;
    int i, j;
    spd_t temp;
    role_t* prole = &rt->role[0];
    enemy_t* penemy = &et->enemy[0];

    for (i = 0; i < rt->num; i++)
    {
        pspdList[i].camp = ROLE;
        pspdList[i].index = i;
        pspdList[i].speed = prole[i].attr.SPD;
    }
    sum = sum + rt->num;
    for (i = 0; i < et->num; i++)
    {
        pspdList[i + sum].camp = ENEMY;
        pspdList[i + sum].index = i;
        pspdList[i + sum].speed = penemy[i].attr.SPD;
    }
    sum = sum + et->num;

    for (i = 1; i < sum; i++)
    {
        temp = pspdList[i];
        j = i - 1;
        while ((j >= 0) && (pspdList[j].speed < temp.speed))
        {
            pspdList[j + 1] = pspdList[j];
            j--;
        }
        pspdList[j + 1] = temp;
    }

}

void DB_drawAttackIcon(COORD sp)
{
    Console_gotoxy(sp.X, sp.Y);
    Console_setColor(FORE_RED, 0);
    Sys_printf("\u27A4\u27A4");/*➤➤*/
    Console_setColor(FORE_WHITE, 0);
}

void DB_clearAttackIcon(COORD sp)
{
    Console_gotoxy(sp.X, sp.Y);
    Sys_printf("    ");/*➤➤*/
}

u8_t DB_attackAction(char_t * msg, role_t* prole, enemy_t* penemy, bool_t passive)
{
    u8_t ret = 0;
    f32_t defenseGain, attackGain;
    /* 默认都是1 */
    defenseGain = 1;
    attackGain = 1;
    /* 攻击 */
    if (passive == FALSE)
    {
        f32_t temp = (f32_t)(prole->batAttr.attackVal - penemy->batAttr.defenseVal);
        f32_t realDEX = (prole->batAttr.DEX - penemy->batAttr.AGL) * 100.0f;
        f32_t realCRIT = prole->batAttr.CRIT * 100.0f;
        f32_t rand_agl = (f32_t)(rand()%100);
        f32_t rand_crit = (f32_t)(rand()%100);
        if (rand_agl < realDEX)
        {
            temp = 0;
            Sys_sprintf(msg, "%s闪避了%s的攻击", penemy->name, prole->name);
        }
        else if (rand_crit < realCRIT)
        {
            temp *= ROLE_CRIT_COEFF;
            Sys_sprintf(msg, "%s暴击了，%s受到了伤害%u", prole->name, penemy->name, (u32_t)temp);
        }
        else
        {
            Sys_sprintf(msg, "%s受到了伤害%u", penemy->name, (u32_t)temp);
        }
        penemy->attr.currentBlood -= (u32_t)temp;
    }
    else /* 被攻击 */
    {
        f32_t temp = penemy->batAttr.attackVal - prole->batAttr.defenseVal;
        f32_t realDEX = (penemy->batAttr.DEX - prole->batAttr.AGL) * 100;
        f32_t realCRIT = penemy->batAttr.CRIT * 100;
        f32_t rand_agl = (f32_t)(rand() % 100);
        f32_t rand_crit = (f32_t)(rand() % 100);
        if (rand_agl > realDEX)
        {
            temp = 0;
            Sys_sprintf(msg, "%s闪避了%s的攻击", penemy->name, prole->name);
        }
        else if (rand_crit < realCRIT)
        {
            temp *= 2;
            Sys_sprintf(msg, "%s暴击了，%s受到了伤害%u", penemy->name, prole->name, (u32_t)temp);
        }
        else
        {
            Sys_sprintf(msg, "%s受到了伤害%u", prole->name, (u32_t)temp);
        }
        prole->attr.currentBlood -= (u32_t)temp;
    }
    return ret;
}


void DB_excuteActionCommand(roleTeam_t* rt, enemyTeam_t* et)
{
    u8_t sum = rt->num + et->num;
    role_t* prole = &rt->role[0];
    enemy_t* penemy = &et->enemy[0];
    char_t msg[50];
    memset(&actionList, 0, sizeof(actionList));
    DB_sortSpd(&actionList[0], rt, et);
    DB_updateBatAttr(rt, et);
    for (u8_t i = 0; i < sum; i++)
    {
        if (actionList[i].camp == ROLE)
        {
            u8_t id = actionList[i].index;
            prole = &rt->role[id];
            if (prole->isValid == TRUE)
            {
                DB_berserker(prole, rt, et);
            }
        }
        else
        {
            u8_t id = actionList[i].index;
            penemy = &et->enemy[id];
            u32_t *EMCMD = (u32_t *)penemy[id].actionCommand;
        }
        Sleep(500);
        DB_gotoBattleBuff(0);
        Sleep(1000);
    }
    DB_gotoBattleBuff(0);
    Sys_printf("新回合开始");
}

void DB_updateBatAttr(roleTeam_t* rt, enemyTeam_t* et)
{
    for (int i = 0; i < rt->num; i++)
    {
        memcpy(&rt->role[i].batAttr, &rt->role[i].attr, sizeof(rt->role[i].attr));
    }

    for (int i = 0; i < et->num; i++)
    {
        memcpy(&et->enemy[i].batAttr, &et->enemy[i].attr, sizeof(et->enemy[i].attr));
    }
}

void DB_calBatAttr(role_t *prole)
{
    if (prole->buff.attackup1.bKeep == TRUE)
    {
        prole->buff.attackup1.cntr--;
        prole->batAttr.attackVal *= 1.5f;
        if (prole->buff.attackup1.cntr == 0)
        {
            prole->buff.attackup1.bKeep = FALSE;
        }
    }

    if (prole->buff.defenceup1.bKeep == TRUE)
    {
        prole->buff.defenceup1.cntr--;
        prole->batAttr.defenseVal *= 1.5f;
        if (prole->buff.defenceup1.cntr == 0)
        {
            prole->buff.defenceup1.bKeep = FALSE;
        }
    }

    if (prole->buff.dodgeup1.bKeep == TRUE)
    {
        prole->buff.dodgeup1.cntr--;
        prole->batAttr.CRIT *= 1.5f;
        if (prole->buff.dodgeup1.cntr == 0)
        {
            prole->buff.dodgeup1.bKeep = FALSE;
        }
    }
}

void DB_healProp(char_t* msg, role_t* dstprole, healProp_t* prop)
{
    healProp_t healpropTemp = *(healProp_t*)prop;
    if ((healpropTemp.aHp != 0) && (healpropTemp.aMp != 0))
    {
        Sys_sprintf(msg, "HP恢复%d, MP恢复%d", healpropTemp.aHp, healpropTemp.aMp);
    }
    else if (healpropTemp.aHp != 0)
    {
        Sys_sprintf(msg, "HP恢复%d", healpropTemp.aHp);
    }
    else
    {
        Sys_sprintf(msg, "MP恢复%d", healpropTemp.aMp);
    }
    dstprole->attr.currentBlood += healpropTemp.aHp;
    if (dstprole->attr.currentBlood > dstprole->attr.maxBlood)
    {
        dstprole->attr.currentBlood = dstprole->attr.maxBlood;
    }
    dstprole->attr.currentMana += healpropTemp.aMp;
    if (dstprole->attr.currentMana > dstprole->attr.maxMana)
    {
        dstprole->attr.currentMana = dstprole->attr.maxMana;
    }
}

void DB_cureProp(char_t* msg, role_t* dstprole, cureProp_t* prop)
{
    cureProp_t curepropTemp = *(cureProp_t*)prop;
    switch (curepropTemp.effect)
    {
    case CUREPETRIFACTION:
        dstprole->debuff.petrifaction.bKeep = FALSE;
        dstprole->debuff.petrifaction.cntr = 0;
        Sys_sprintf(msg, "解除了石化，%s恢复正常", dstprole->name);
        break;
    case CUREPARALYSIS:

        dstprole->debuff.paralysis.bKeep = FALSE;
        dstprole->debuff.paralysis.cntr = 0;

        Sys_sprintf(msg, "解除了石化，%s恢复正常", dstprole->name);
        break;
    case CURELACERATE:

        dstprole->debuff.lacerate.bKeep = FALSE;
        dstprole->debuff.lacerate.cntr = 0;

        Sys_sprintf(msg, "止住流血，%s恢复正常", dstprole->name);
        break;
    case CUREINTOXICATION:

        dstprole->debuff.intoxication.bKeep = FALSE;
        dstprole->debuff.intoxication.cntr = 0;

        Sys_sprintf(msg, "解除了中毒，%s恢复正常", dstprole->name);
        break;
    case CUREBLIND:

        dstprole->debuff.blind.bKeep = FALSE;
        dstprole->debuff.blind.cntr = 0;

        Sys_sprintf(msg, "解除了失明，%s恢复正常", dstprole->name);
        break;
    case CUREALL:

        dstprole->debuff.petrifaction.bKeep = FALSE;
        dstprole->debuff.petrifaction.cntr = 0;

        dstprole->debuff.paralysis.bKeep = FALSE;
        dstprole->debuff.paralysis.cntr = 0;

        dstprole->debuff.lacerate.bKeep = FALSE;
        dstprole->debuff.lacerate.cntr = 0;

        dstprole->debuff.intoxication.bKeep = FALSE;
        dstprole->debuff.intoxication.cntr = 0;

        dstprole->debuff.blind.bKeep = FALSE;
        dstprole->debuff.blind.cntr = 0;

        Sys_sprintf(msg, "%s恢复正常", dstprole->name);
        break;
    default:
        break;
    }
}

void DB_mulityFuncProp(char_t* msg, role_t* dstprole, mulityFuncProp_t* prop)
{

}

void DB_isEnemyDead(enemy_t* penemy)
{
    if (penemy->attr.currentBlood <= 0)
    {
        penemy->isValid = FALSE;
    }
}

void DB_damageSkill(char_t* msg, damageSkill_t* skill, role_t* prole, enemy_t* penemy)
{
    f32_t damage, CRIT;

    damage = skill->damage + skill->damCoeff * prole->batAttr.attackVal;
    CRIT = (f32_t)(rand() % 100);
    if (CRIT < prole->batAttr.CRIT * 100)
    {
        damage *= ROLE_CRIT_COEFF;
    }
    damage -= penemy->batAttr.defenseVal;
    if (damage < 0)
    {
        damage = 0;
    }
    penemy->attr.currentBlood -= (u32_t)damage;
    Sys_sprintf(msg, "对%s造成伤害%d",penemy->name, (u32_t)damage);
}

/* 狂战士 */
u8_t DB_berserker(role_t* prole, roleTeam_t* rt, enemyTeam_t* et)
{
    u8_t ret = 0;
    u8_t tempId;
    u8_t objId;
    char_t msg[50];
    roleBatCmd_t *ROCMD = (roleBatCmd_t*)prole->actionCommand;
    enemy_t* penemy = &et->enemy[0];
    DB_gotoBattleBuff(0);
    DB_calBatAttr(prole);
    switch (ROCMD->kind)
    {
    case actionEnum_t::ACTATTACK:
        if (1)
        {
            tempId = ROCMD->executedObjId;
            penemy = &et->enemy[tempId];
            if (penemy->isValid == FALSE)
            {
                u8_t id = DB_getMappedEnemyId(et, 0);
                penemy = &et->enemy[id];
            }
            Sys_sprintf(msg, "%s 向 %s 发动攻击", prole->name, penemy->name);
            DB_writeBattleBuff(msg, TRUE);
            Sleep(BATTLE_TIME_DELAY);
            DB_attackAction(msg, prole, penemy, FALSE);
            DB_writeBattleBuff(msg, FALSE);
            DB_updateEnemy(penemy);
            Sleep(BATTLE_TIME_DELAY);
            DB_isEnemyDead(penemy);
            if (penemy->isValid == FALSE)
            {
                Sys_sprintf(msg, " %s被打败了", penemy->name);
                DB_writeBattleBuff(msg, FALSE);
            }
            Sleep(BATTLE_TIME_DELAY);
        }
        break;
    case actionEnum_t::ACTDEFEND:
        Sys_sprintf(msg, "%s 正在防御", prole->name);
        DB_writeBattleBuff(msg, TRUE);
        break;
    case actionEnum_t::ACTPROPUSED:
        tempId = ROCMD->propId;
        objId = ROCMD->executedObjId;
        switch (propAll[tempId].type)
        {
        case HEALPROP:
            tempId = propAll[tempId].dstId;
            {
                healProp_t propTemp = healPropList[tempId];
                role_t* roleTemp = &rt->role[objId];
                Sys_sprintf(msg, "%s 使用了 %s", roleTemp->name, propTemp.name);
                DB_writeBattleBuff(msg, TRUE);
                DB_healProp(msg, roleTemp, &propTemp);
                DB_writeBattleBuff(msg, FALSE);
            }
            break;
        case CUREPROP:
            tempId = propAll[tempId].dstId;
            {
                cureProp_t propTemp = curePropList[tempId];
                role_t* roleTemp = &rt->role[objId];
                Sys_sprintf(msg, "%s 使用了 %s", roleTemp->name, propTemp.name);
                DB_writeBattleBuff(msg, TRUE);
                DB_cureProp(msg, roleTemp, &propTemp);
                DB_writeBattleBuff(msg, FALSE);
            }
            break;
        case MULITYFUNCPROP:
            tempId = propAll[tempId].dstId;
            {
                mulityFuncProp_t propTemp = mfPropList[tempId];

                role_t* roleTemp = &rt->role[objId];
                Sys_sprintf(msg, "%s 使用了 %s", roleTemp->name, propTemp.name);
                DB_writeBattleBuff(msg, TRUE);

                healProp_t healpropTemp;
                healpropTemp.aHp = propTemp.aHp;
                healpropTemp.aMp = propTemp.aMp;
                DB_healProp(msg, roleTemp, &healpropTemp);
                DB_writeBattleBuff(msg, FALSE);

                cureProp_t curepropTemp;
                curepropTemp.effect = propTemp.effect;
                DB_cureProp(msg, roleTemp, &curepropTemp);
                DB_writeBattleBuff(msg, FALSE);
            }
            break;
        case DAMAGEPROP:
            break;
        default:
            break;
        }
        break;
    case actionEnum_t::ACTSKILLUSED:
        tempId = ROCMD->skillId;
        objId = ROCMD->executedObjId;
        {
            damageSkill_t skillTemp = damSkillList[tempId];
            Sys_sprintf(msg, " %s 释放了 %s", prole->name, skillTemp.name);
            DB_writeBattleBuff(msg, TRUE);
            for (int i = 0; i < skillTemp.attackCntr; i++)
            {
                if (penemy->isValid == FALSE)
                {
                    u8_t id = DB_getMappedEnemyId(et, 0);
                    penemy = &et->enemy[id];
                }
                DB_damageSkill(msg, &skillTemp, prole, penemy);
                DB_writeBattleBuff(msg, FALSE);
                DB_updateEnemy(penemy);
                
            }
        }
        break;
    case actionEnum_t::ACTESCAPE:
        ret = 1;
        Sys_sprintf(msg, "%s 逃跑了", prole->name);
        DB_writeBattleBuff(msg, TRUE);
        break;
    default:
        break;
    }
    return ret;
}


#ifdef _BIT_MASK_COMMAND
u32_t DB_setValidBit(u32_t command, bool_t isValid)
{
    command &= (u32_t)~(1 << 31);
    command |= ((u32_t)isValid << 31);
    return command;
}

bool_t DB_getValidBit(u32_t command)
{
    bool_t isValid;
    isValid = command >> 31;
    return isValid;
}
u32_t DB_setIsEntireBit(u32_t command, bool_t isValid)
{
    command &= (u32_t)~(1 << 5);
    command |= ((u32_t)isValid << 5);
    return command;
}

bool_t DB_getIsEntireBit(u32_t command)
{
    bool_t isValid;
    isValid = (command >> 5) & 1;
    return isValid;
}
u32_t DB_setIsRoleBit(u32_t command, bool_t isValid)
{
    command &= (u32_t)~(1 << 6);
    command |= ((u32_t)isValid << 6);
    return command;
}
bool_t DB_getIsRoleBit(u32_t command)
{
    bool_t isValid;
    isValid = (command >> 6) & 1;
    return isValid;
}
/* 
     
     (1 << (start bit - end bit + 1) - 1)
*/

u32_t DB_setActionBit(u32_t command, u32_t action)
{
    command &= (u32_t)~(((1u << 3) - 1) << 28);
    command |= (action << 28);
    return command;
}

u32_t DB_getActionBit(u32_t command)
{
    u32_t action;
    action = (command >> 28)& (u32_t)((1u << 3) - 1);
    return action;
}

u32_t DB_setActionIndexBit(u32_t command, u32_t index)
{
    command &= (u32_t)~(((1u << (15 -7 + 1)) - 1) << 7);
    command |= (index << 7);
    return command;
}


u32_t DB_getActionIndexBit(u32_t command)
{
    u32_t index;
    index = (command >> 7) & (u32_t)((1u << (15 - 7 + 1)) - 1);
    return index;
}

u32_t DB_setActionObjectBit(u32_t command, u32_t index)
{
    command &= (u32_t)~(((1u << 3) - 1) << 0);
    command |= (index << 0);
    return command;
}

u32_t DB_getActionObjectBit(u32_t command)
{
    u32_t index;
    index = (command >> 0) & (u32_t)((1u << 3) - 1);
    return index;
}
#endif


