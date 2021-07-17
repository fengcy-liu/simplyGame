#pragma once
#ifndef DISPLAY_BATTLE_H
#define DISPLAY_BATTLE_H

#include "console.h"
#include "common_type.h"
#include "role.h"
#include "displayMenu.h"
#include "public_cfg.h"



#define BATTLE_BUTTON_NUM                                  5u

#define BATTLE_ACTION_ESCAPE_MASK_VAL                      0u
#define BATTLE_ACTION_ATTACK_MASK_VAL                      1u
#define BATTLE_ACTION_DEFEND_MASK_VAL                      2u
#define BATTLE_ACTION_PROP_MASK_VAL                        3u
#define BATTLE_ACTION_SKILL_MASK_VAL                       4u

#define BATTLE_TIME_DELAY                                  500

typedef struct spdTag
{
    u8_t index;
    f32_t speed;
    enumTeamType_t camp;
}spd_t;

typedef enum class actionEnumTag
{
    ACTATTACK=0,
    ACTDEFEND,
    ACTPROPUSED,
    ACTSKILLUSED,
    ACTESCAPE
}actionEnum_t;

typedef struct roleBatCmdTag
{

    actionEnum_t kind;
    spd_t executorInfo;
    u8_t executedObjId;
    u8_t propId;
    u8_t skillId;
}roleBatCmd_t;

void DB_initBattleScreen(roleTeam_t *rt, enemyTeam_t *et);
void DB_drawRoleTeam(roleTeam_t* rt);
void DB_drawEnemyTeam(enemyTeam_t* et);
void DB_updateBattleOpMenu(int preIndex, int curIndex);
u8_t DB_getValidRole(roleTeam_t *rt);
u8_t DB_getValidEnemy(enemyTeam_t* et);
void DB_writeBattleBuff(char_t* str, bool_t bClean);
void DB_gotoBattleBuff(u8_t index = 0);
void DB_clearBattleBuff(void);
void DB_drawRoleNameContext(char_t *roleName);
void DB_clearRoleNameContext(void);
void DB_sortSpd(spd_t* pspdList, roleTeam_t* rt, enemyTeam_t* et);

void DB_updateRole(role_t* prole);
void DB_updateEnemy(enemy_t* penemy);

void DB_drawAttackIcon(COORD sp);
void DB_clearAttackIcon(COORD sp);
u8_t DB_getMappedEnemyId(enemyTeam_t* et, u8_t index);
u8_t DB_getMappedRoleId(roleTeam_t* rt, u8_t index);

u8_t DB_attackAction(char_t *msg, role_t *prole, enemy_t *penemy, bool_t passive);
void DB_excuteActionCommand(roleTeam_t* rt, enemyTeam_t* et);

#ifdef _BIT_MASK_COMMAND
u32_t DB_setValidBit(u32_t command, bool_t isValid);
bool_t DB_getValidBit(u32_t command);
u32_t DB_setActionBit(u32_t command, u32_t action);
u32_t DB_getActionBit(u32_t command);
u32_t DB_setIsEntireBit(u32_t command, bool_t isValid);
bool_t DB_getIsEntireBit(u32_t command);
u32_t DB_setIsRoleBit(u32_t command, bool_t isValid);
bool_t DB_getIsRoleBit(u32_t command);
u32_t DB_setActionIndexBit(u32_t command, u32_t index);
u32_t DB_getActionIndexBit(u32_t command);
u32_t DB_setActionObjectBit(u32_t command, u32_t index);
u32_t DB_getActionObjectBit(u32_t command);
#endif // _BIT_MASK_COMMAND


#endif // !DISPLAY_BATTLE_H
