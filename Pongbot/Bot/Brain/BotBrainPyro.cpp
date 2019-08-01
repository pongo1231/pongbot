#include <stdafx.h>
#include "BotBrainPyro.h"
#include "../Visibles/BotVisibles.h"
#include "Tasks/BotTaskAggressiveCombat.h"
#include "../TF2/Entity/Entity.h"

void BotBrainPyro::_OnThink()
{
	Bot* bot = _GetBot();

	BotVisibleTarget* currentTarget = bot->GetBotVisibles()->GetMostImportantTarget();
	if (currentTarget && bot->GetSelectedWeaponSlot() == WEAPON_PRIMARY)
	{
		if (!_IsRushingEnemy)
		{
			_IsRushingEnemy = true;
			_SetBotTask(new BotTaskAggressiveCombat(bot, currentTarget->GetEntity(), WEAPON_PRIMARY));
		}
	}
	else
	{
		_IsRushingEnemy = false;
	}
}

void BotBrainPyro::_OnSpawn()
{
	_IsRushingEnemy = false;
}