#pragma once
#include "BotTask.h"
#include "../TF2/Entity/Entity.h"

class BotTaskAggressiveCombat : public BotTask
{
public:
	BotTaskAggressiveCombat(Bot* bot, Entity targetEntity, WeaponSlot weaponSlot) : BotTask(bot),
		_TargetEntity(targetEntity), _MWeaponSlot(weaponSlot) {}

private:
	const Entity _TargetEntity;
	const WeaponSlot _MWeaponSlot;

	virtual bool _OnThink();
};