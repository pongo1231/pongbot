#include <stdafx.h>
#include "BotBrainPyro.h"
#include "../Visibles/BotVisibles.h"
#include "Tasks/BotTaskAggressiveCombat.h"
#include "../../TF2/Entity/Entity.h"

void BotBrainPyro::_OnThink()
{
	Bot* bot = _GetBot();

	BotVisibleTarget currentTarget = bot->GetBotVisibles()->GetMostImportantTarget();
	if (currentTarget.IsValid() && bot->GetSelectedWeaponSlot() == WEAPON_PRIMARY
		&& !_IsCurrentBotTaskOfType(typeid(BotTaskAggressiveCombat)))
	{
		_SetBotTask(new BotTaskAggressiveCombat(bot, currentTarget.GetEntity(), WEAPON_PRIMARY));
	}
}