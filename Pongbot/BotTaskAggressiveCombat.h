#pragma once
#include "BotTask.h"

struct edict_t;

class BotTaskAggressiveCombat : public BotTask
{
public:
	BotTaskAggressiveCombat(Bot *bot, edict_t *targetEdict, WeaponSlot weaponSlot) : BotTask(bot),
		_TargetEdict(targetEdict), _MWeaponSlot(weaponSlot)
	{}

private:
	edict_t *_TargetEdict;
	WeaponSlot _MWeaponSlot;

	virtual bool _OnThink();
};