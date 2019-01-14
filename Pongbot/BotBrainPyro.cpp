#include "BotBrainPyro.h"
#include "BotVisibles.h"
#include "BotTaskAggressiveCombat.h"
#include "Entity.h"

bool _IsRushingEnemy = false;

void BotBrainPyro::_OnThink()
{
	Bot *bot = _GetBot();

	BotVisibleTarget *currentTarget = bot->GetBotVisibles()->GetMostImportantTarget();
	if (currentTarget && bot->GetSelectedWeaponSlot() == WEAPON_PRIMARY)
	{
		if (!_IsRushingEnemy)
		{
			_IsRushingEnemy = true;
			_SetBotTask(new BotTaskAggressiveCombat(bot, currentTarget->Entity, WEAPON_PRIMARY));
		}
	}
	else
		_IsRushingEnemy = false;
}

void BotBrainPyro::_OnSpawn()
{
	_IsRushingEnemy = false;
}