#include "BotBrainPyro.h"
#include "BotVisibles.h"
#include "BotTaskAggressiveCombat.h"
#include "Util.h"

void BotBrainPyro::_OnThink()
{
	Bot *bot = _GetBot();
	std::queue<BotTask*> newTaskQueue;

	BotVisibleTarget *currentTarget = bot->GetBotVisibles()->GetMostImportantTarget();
	if (currentTarget && bot->GetSelectedWeaponSlot() == WEAPON_PRIMARY && !_HasState(BOTSTATE_PYRO_FLAMETHROWERRUSH))
	{
		_AddState(BOTSTATE_PYRO_FLAMETHROWERRUSH);
		newTaskQueue.push(new BotTaskAggressiveCombat(bot, currentTarget->Edict, WEAPON_PRIMARY));
	}
	else if (_HasState(BOTSTATE_PYRO_FLAMETHROWERRUSH))
		_RemoveState(BOTSTATE_PYRO_FLAMETHROWERRUSH);

	if (!newTaskQueue.empty())
		SetTaskQueue(newTaskQueue);
}