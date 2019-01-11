#include "BotBrainPyro.h"
#include "BotVisibles.h"
#include "BotTaskAggressiveCombat.h"

bool _IsRushingEnemy;

void BotBrainPyro::_OnThink()
{
	Bot *bot = _GetBot();
	std::queue<BotTask*> newTaskQueue;

	BotVisibleTarget *currentTarget = bot->GetBotVisibles()->GetMostImportantTarget();
	if (currentTarget && bot->GetSelectedWeaponSlot() == WEAPON_PRIMARY)
	{
		if (!_IsRushingEnemy)
		{
			_IsRushingEnemy = true;
			newTaskQueue.push(new BotTaskAggressiveCombat(bot, currentTarget->Edict, WEAPON_PRIMARY));
		}
	}
	else
		_IsRushingEnemy = false;

	if (!newTaskQueue.empty())
		SetTaskQueue(newTaskQueue);
}

void BotBrainPyro::_OnSpawn()
{
	_IsRushingEnemy = false;
}