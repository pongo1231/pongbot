#include <stdafx.h>
#include "BotBrainSniper.h"
#include "../Visibles/BotVisibles.h"
#include "../../ConVarHolder.h"
#include "Tasks/BotTaskSniperSnipe.h"
#include "../../Util.h"
#include "../../Waypoint/WaypointManager.h"
#include "../../Waypoint/WaypointNodeFlagTypes.h"
#include "Tasks/BotTaskGoto.h"

extern WaypointManager* _WaypointManager;

void BotBrainSniper::_OnThink()
{
	Bot* bot = _GetBot();
	Vector botPos = bot->GetPos();
	TFTeam botTeam = bot->GetTeam();

	if (!_IsCurrentBotTaskOfType(typeid(BotTaskSniperSnipe)) && bot->GetPlayer().IsSniperZoomedIn())
	{
		bot->SetPressedButtons(IN_ATTACK2);
		return;
	}

	BotVisibleTarget visibleTarget = bot->GetBotVisibles()->GetMostImportantTarget();
	if (visibleTarget.IsValid()
		&& Util::DistanceToNoZ(botPos, visibleTarget.GetPos()) > _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat()
		&& !_IsCurrentBotTaskOfType(typeid(BotTaskSniperSnipe)))
	{
		_SetBotTask(new BotTaskSniperSnipe(bot));
	}

	if (!_HasBotTask())
	{
		WaypointNode* node = _WaypointManager->GetClosestWaypointNode(botPos);
		if (node && node->Flags & (botTeam == TEAM_RED ? NODE_SNIPER_CAMP_RED : NODE_SNIPER_CAMP_BLUE)
			&& botPos.DistTo(node->GetPos()) < node->GetRange())
		{
			_SetBotTask(new BotTaskSniperSnipe(bot, _ConVarHolder->CVarBotSniperCampTime->GetInt(), node->OptimalViewAngle));
		}
		else
		{
			node = _WaypointManager->GetRandomWaypointNode(botTeam == TEAM_RED ? ~NODE_SNIPER_CAMP_RED : ~NODE_SNIPER_CAMP_BLUE);
			if (node)
			{
				_SetBotTask(new BotTaskGoto(bot, node->GetPos()));
			}
		}
	}
}