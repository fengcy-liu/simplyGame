// gameHardWorld.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "console.h"
#include "displayMenu.h"
#include "operator.h"
#include "gameHardWorld.h"
#include "displayBattle.h"
#include "role.h"

void startMenu(void);
void battlePlay(void);
void gameStart(void);
void gameLoad(void);
void gameSetting(void);
void gameExit(void);

void battlePlayDemo(void);

roleBatCmd_t batRoleCmd[ROLE_MAX_ROLE_NUM_OF_TEAM];
u32_t batEmenyCmd[ROLE_MAX_NUM_OF_ENEMY];
static bool_t gameExitFlag = FALSE;

void gameStart(void)
{
    battlePlayDemo();
}

void gameLoad(void)
{
    Console_gotoxy(0, 6);
    Sys_printf("待定");
}

void gameSetting(void)
{
    Console_gotoxy(0, 6);
    Sys_printf("待定");
}

void gameExit(void)
{
    gameExitFlag = TRUE;
    Console_clearAll();
    Sys_printf("游戏结束！");
}

void mainGame(void)
{
    
    DM_initConsloe();
    DM_initCfg();
    startMenu();

}

void startMenu(void)
{
    DM_initStartMenu();
    while (!gameExitFlag)
    {
        static opSts_t event;
        static int lastPosY;
        lastPosY = event.pos.Y;
        if (TRUE == OP_startMenu(&event))
        {
            Console_gotoxy(0, 7);
            Sys_printf("pos : %d", event.pos.Y);
            if (event.isDone == TRUE)
            {
                event.isDone = FALSE;
                switch (event.pos.Y)
                {
                case 0:
                    gameStart();
                    break;
                case 1:
                    gameLoad();
                    break;
                case 2:
                    gameSetting();
                    break;
                case 3:
                    gameExit();
                    break;
                default:
                    break;
                }
            }
            else
            {
                DM_updateStartMenu(lastPosY, event.pos.Y);
                //debug
                Console_gotoxy(0, 6);
                Sys_printf("    ");
            }
        }
    }
}

void battlePlayDemo(void)
{
    roleTeam_t a;
    enemyTeam_t b;

    roleTeam_t* prtList = &a;
    enemyTeam_t* petList = &b;
    opSts_t event;
    bool_t isPress = FALSE;
    bool_t battleExitFlag = FALSE;
    int preId = 0;
    memset(&a, 0, sizeof(a));
    memset(&b, 0, sizeof(b));
    memset(&event, 0, sizeof(event));
    DM_initConsloe();
    a.num = 2;
    a.userProp.propNum = 4;
    for (int i = 0; i < a.userProp.propNum; i++)
    {
        a.userProp.list[i] = i;
    }
    a.userProp.keepCntr[0] = 5;
    a.userProp.keepCntr[1] = 2;
    a.userProp.keepCntr[2] = 5;
    a.userProp.keepCntr[3] = 1;

    for (int i = 0; i < a.num; i++)
    {
        Sys_sprintf(a.role[i].name, "测试员%d号", i);
        a.role[i].skillIdNum = 1;
        a.role[i].attr.attackVal = 100;
        a.role[i].skillIdList[0] = 0;
        a.role[i].attr.maxBlood = 100 * i + 100;
        a.role[i].attr.currentBlood = a.role[i].attr.maxBlood;
        a.role[i].attr.SPD = i + 4;
        a.role[i].quene = i;
        a.role[i].isValid = TRUE;
    }
    b.num = 4;
    for (int i = 0; i < b.num; i++)
    {
        Sys_sprintf(b.enemy[i].name, "测试怪物%d号", i);
        b.enemy[i].attr.maxBlood = 100 * i + 100;
        b.enemy[i].attr.currentBlood = b.enemy[i].attr.maxBlood;
        b.enemy[i].attr.SPD = i;
        b.enemy[i].quene = i;
        b.enemy[i].isValid = TRUE;
    }

    DB_initBattleScreen(prtList, petList);

    role_t* prole = &prtList->role[0];
    enemy_t* penemy = &petList->enemy[0];

    while (!battleExitFlag)
    {
        bool_t endTurn;
        u8_t index, quene, preopCntr, opCntr;
        u8_t maxOpCntr, maxEnemyCntr;
        opSts_t actEvent;
        endTurn = FALSE;
        opCntr = 0;
        maxOpCntr = DB_getValidRole(prtList);
        maxEnemyCntr = DB_getValidEnemy(petList);
        while (!endTurn)
        {
            for (int i = 0; i < prtList->num; i++)
            {
                if (opCntr == prtList->role[i].quene)
                {
                    prole = &prtList->role[i];
                    DB_drawRoleNameContext(prole->name);
                    break;
                }
            }
            memset(&actEvent, 0, sizeof(actEvent));
            if (OP_battleOpMenu(&event))
            {
                if (event.isDone == TRUE)
                {
                    preopCntr = opCntr;
                    opCntr++;
                    DB_gotoBattleBuff();

                    event.isDone = FALSE;
                    switch (event.pos.Y)
                    {
                        /* 攻击 */
                    case 0:
                        index = DB_getMappedEnemyId(petList, (u8_t)actEvent.pos.Y);
                        COORD temp;
                        temp = petList->enemy[index].battlePostion;
                        temp.X -= 4;
                        DB_drawAttackIcon(temp);
                        while ((!actEvent.isDone) && (!actEvent.isCancel))
                        {
                            if (TRUE == OP_battleAction(&actEvent, maxEnemyCntr))
                            {
                                if (actEvent.isDone == FALSE)
                                {
                                    if (actEvent.isCancel == TRUE)
                                    {
                                        opCntr--;
                                    }
                                    else
                                    {
                                        DB_clearAttackIcon(temp);
                                        index = DB_getMappedEnemyId(petList, (u8_t)actEvent.pos.Y);
                                        temp = petList->enemy[index].battlePostion;
                                        temp.X -= 4;
                                        DB_drawAttackIcon(temp);
                                    }
                                }
                                else
                                {
                                    batRoleCmd[opCntr - 1].kind = actionEnum_t::ACTATTACK;
                                    batRoleCmd[opCntr - 1].executedObjId = index;
                                }
                            }
                        }
                        DB_clearAttackIcon(temp);
                        break;
                        /* 防御 */
                    case 1:
                        batRoleCmd[opCntr - 1].kind = actionEnum_t::ACTDEFEND;
                        break;
                        /* 技能 */
                    case 2:
                        DB_gotoBattleBuff(0);
                        for (int i = 0; i < prole->skillIdNum; i++)
                        {
                            u8_t temp = prole->skillIdList[i];
                            Sys_printf("%s", damSkillList[temp].name);
                        }
                        break;
                        /* 物品 */
                    case 3:
                        break;
                        /* 逃跑 ， */
                    case 4:

                        batRoleCmd[opCntr - 1].kind = actionEnum_t::ACTESCAPE;
                        //DB_gotoBattleBuff();
                        //Sys_printf("懦夫");
                        break;
                    default:
                        break;
                    }
                    if (opCntr > preopCntr)
                    {
                        prole->actionCommand = (void *)&batRoleCmd[opCntr - 1];
                    }
                    else
                    {
                        prole->actionCommand = NULL;
                    }
                }
                else if (event.isCancel == TRUE)
                {
                    event.isCancel = FALSE;
                    if (opCntr > 0)
                    {
                        opCntr--;
                    }
                    DB_gotoBattleBuff(0);
                    Sys_printf("%d", opCntr);
                }
                else
                {
                    DB_gotoBattleBuff(0);
                    DB_updateBattleOpMenu(preId, event.pos.Y);
                    preId = event.pos.Y;
                }

                if (opCntr >= maxOpCntr)
                {
                    endTurn = TRUE;
                    DB_clearRoleNameContext();
                    DB_gotoBattleBuff(0);
                    Sys_printf("战斗开始");
                }
            }
        }
        DB_excuteActionCommand(prtList, petList);
    }
}

#if 0 
void battlePlay(void)
{
    roleTeam_t a;
    enemyTeam_t b;

    roleTeam_t* prtList = &a;
    enemyTeam_t* petList = &b;
    opSts_t event;
    bool_t isPress = FALSE;
    bool_t battleExitFlag = FALSE;
    int preId = 0;
    memset(&a, 0, sizeof(a));
    memset(&b, 0, sizeof(b));
    memset(&event, 0, sizeof(event));
    DM_initConsloe();
    a.num = 3;
    for (int i = 0; i < a.num; i++)
    {
        Sys_sprintf(a.role[i].name, "测试员%d号", i);
        a.role[i].attr.maxBlood = 100 * i + 100;
        a.role[i].attr.currentBlood = a.role[i].attr.maxBlood;
        a.role[i].attr.SPD = i + 4;
        a.role[i].quene = i;
        a.role[i].isValid = TRUE;
    }
    b.num = 4;
    for (int i = 0; i < b.num; i++)
    {
        Sys_sprintf(b.enemy[i].name, "测试怪物%d号", i);
        b.enemy[i].attr.maxBlood = 100 * i + 100;
        b.enemy[i].attr.currentBlood = b.enemy[i].attr.maxBlood;
        b.enemy[i].attr.SPD = i;
        b.enemy[i].quene = i;
        b.enemy[i].isValid = TRUE;
    }
    
    DB_initBattleScreen(prtList, petList);

    role_t *prole = &prtList->role[0];
    enemy_t *penemy = &petList->enemy[0];

    while (!battleExitFlag)
    {
        bool_t endTurn;
        u8_t index, quene, preopCntr, opCntr;
        u8_t maxOpCntr, maxEnemyCntr;
        opSts_t actEvent;
        endTurn = FALSE;
        opCntr = 0;
        maxOpCntr = DB_getValidRole(prtList); 
        maxEnemyCntr = DB_getValidEnemy(petList);

        while (!endTurn)
        {
            for (int i = 0; i < prtList->num; i++)
            {
                if (opCntr == prtList->role[i].quene)
                {
                    prole = &prtList->role[i];
                    DB_drawRoleNameContext(prole->name);
                    break;
                }
            }
            memset(&actEvent, 0, sizeof(actEvent));
            if (OP_battleOpMenu(&event))
            {
                if (event.isDone == TRUE)
                {
                    preopCntr = opCntr;
                    opCntr++;
                    DB_gotoBattleBuff();

                    event.isDone = FALSE;
                    switch (event.pos.Y)
                    {
                        /* 攻击 */
                    case 0:
                        index = DB_getMappedEnemyId(petList,(u8_t)actEvent.pos.Y);
                        COORD temp;
                        temp= petList->enemy[index].battlePostion;
                        temp.X -= 4;
                        DB_drawAttackIcon(temp);
                        while ((!actEvent.isDone) &&(!actEvent.isCancel) )
                        {
                            if (TRUE == OP_battleAction(&actEvent, maxEnemyCntr))
                            {
                                if (actEvent.isDone == FALSE)
                                {
                                    if (actEvent.isCancel == TRUE)
                                    {
                                        prole->actionCommand = 0;
                                        opCntr--;
                                    }
                                    else
                                    {
                                        DB_clearAttackIcon(temp);
                                        index = DB_getMappedEnemyId(petList, (u8_t)actEvent.pos.Y);
                                        temp = petList->enemy[index].battlePostion;
                                        temp.X -= 4;
                                        DB_drawAttackIcon(temp);
                                    }
                                }
                                else
                                {
                                    prole->actionCommand = DB_setActionBit(prole->actionCommand, BATTLE_ACTION_ATTACK_MASK_VAL);
                                    prole->actionCommand = DB_setActionObjectBit(prole->actionCommand, index);
                                }
                            }
                        }
                        DB_clearAttackIcon(temp);
                        break;
                        /* 防御 */
                    case 1:
                        prole->actionCommand = DB_setActionBit(prole->actionCommand, BATTLE_ACTION_DEFEND_MASK_VAL);
                        break;
                        /* 技能 */
                    case 2:
                        break;
                        /* 物品 */
                    case 3:
                        break;
                        /* 逃跑 ， */
                    case 4:
                        prole->actionCommand = DB_setActionBit(prole->actionCommand, BATTLE_ACTION_ESCAPE_MASK_VAL);
                        //DB_gotoBattleBuff();
                        //Sys_printf("懦夫");
                        break;
                    default:
                        break;
                    }
                    if (opCntr > preopCntr)
                    {
                        prole->actionCommand = DB_setValidBit(prole->actionCommand, TRUE);
                    }
                    else
                    {
                        prole->actionCommand = DB_setValidBit(prole->actionCommand, FALSE);
                    }
                }
                else if (event.isCancel == TRUE)
                {
                    event.isCancel = FALSE;
                    if (opCntr > 0)
                    {
                        opCntr--;
                    }
                    DB_gotoBattleBuff();
                    Sys_printf("%d", opCntr);
                }
                else
                {
                    DB_gotoBattleBuff();
                    Sys_printf("%d", opCntr);
                    DB_updateBattleOpMenu(preId, event.pos.Y);
                    preId = event.pos.Y;
                }

                

                if (opCntr >= maxOpCntr)
                {
                    endTurn = TRUE;
                    DB_gotoBattleBuff();
                    Sys_printf("战斗开始");
                }
            }
        }
        DB_excuteActionCommand(prtList, petList);
    }
}
#endif
