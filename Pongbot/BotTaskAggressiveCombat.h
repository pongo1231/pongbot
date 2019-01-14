#pragma once
#include "BotTask.h"
#include "Entity.h"

class BotTaskAggressiveCombat : public BotTask
{
public:
	BotTaskAggressiveCombat(Bot *bot, Entity targetEntity, WeaponSlot weaponSlot) : BotTask(bot),
		_TargetEdict(targetEntity.GetEdict()), _MWeaponSlot(weaponSlot)
	{}

private:
	edict_t *_TargetEdict;
	WeaponSlot _MWeaponSlot;

	virtual bool _OnThink();
};