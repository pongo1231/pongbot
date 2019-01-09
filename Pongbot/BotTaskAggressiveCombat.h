#pragma once
#include "BotTask.h"

struct edict_t;

class BotTaskAggressiveCombat : public BotTask
{
public:
	BotTaskAggressiveCombat(Bot *bot, edict_t *targetEdict, WeaponSlot weaponSlot);

private:
	edict_t *_TargetEdict;
	IPlayerInfo *_TargetPlayerInfo;
	WeaponSlot _MWeaponSlot;

	virtual bool _OnThink();
};