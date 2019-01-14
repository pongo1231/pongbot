#include "BotBrain.h"
#include "BotTaskGoto.h"
#include "BotTaskAggressiveCombat.h"
#include "WaypointManager.h"
#include "WaypointNodeFlagsProvider.h"
#include "ObjectivesProvider.h"
#include "CTFFlagStatusType.h"
#include "Util.h"
#include "BotVisibles.h"
#include "ConVarHolder.h"
#include "WeaponSlot.h"
#include "CTFFlag.h"
#include <metamod/ISmmAPI.h>
#include <stdint.h> // uint8_t for Linux

extern IVEngineServer *Engine;

Bot *_ABot;
BotTask *_BotTask;
float _ThinkTime = 0.f;
unsigned int _States = 0;
bool _IsBotDead = false;
bool _IsBotInMeleeFight = false;

void BotBrain::OnThink()
{
	if (_ABot->IsDead())
		_IsBotDead = true;
	else
	{
		if (_IsBotDead)
			OnSpawn();

		float engineTime = Engine->Time();
		if (_ThinkTime < engineTime)
		{
			_ThinkTime = engineTime + _ConVarHolder->CVarBotBrainThinkTick->GetFloat();
			_OnThink();
			_DefaultThink();
		}

		if (_HasBotTask() && _BotTask->OnThink())
			_ClearTask();
	}
}

void BotBrain::_DefaultThink()
{
	Vector botPos = _ABot->GetPos();

	/* Check for objectives */

	std::vector<Objective> pushObjectives = _ObjectivesProvider->GetBotPushObjectives(_ABot);
	// Get closest objective
	Objective *closestObjective = nullptr;
	float closestObjectiveDistance = 99999.f;
	for (Objective pushObjective : pushObjectives)
	{
		float distance = botPos.DistTo(pushObjective.Pos);
		if (distance < closestObjectiveDistance)
		{
			delete closestObjective;
			closestObjective = new Objective(pushObjective);
			closestObjectiveDistance = distance;
		}
	}

	/* Tasks which should be able to override current ones */

	// Melee combat
	BotVisibleTarget *currentTarget = _ABot->GetBotVisibles()->GetMostImportantTarget();
	if (currentTarget && _ABot->GetSelectedWeaponSlot() == WEAPON_MELEE)
	{
		if (!_IsBotInMeleeFight)
		{
			_IsBotInMeleeFight = true;
			_SetBotTask(new BotTaskAggressiveCombat(_ABot, currentTarget->Entity, WEAPON_MELEE));
		}
	}
	else
		_IsBotInMeleeFight = false;

	/* Filler Tasks in case the bot has nothing to do */

	if (!_HasBotTask())
	{
		if (closestObjective)
		{
			// CTF Flag stuff
			if (closestObjective->Type == ITEMFLAG)
			{
				CTFFlagStatusType itemFlagStatus = (CTFFlagStatusType) closestObjective->Status;
				if (itemFlagStatus == CTF_UNTOUCHED || itemFlagStatus == CTF_DROPPED) // The flag should be picked up
					_SetBotTask(new BotTaskGoto(_ABot, closestObjective->Pos, false));
				else if (CTFFlag(closestObjective->Edict).GetOwner() == Engine->IndexOfEdict(_ABot->GetEdict()))
				{
					// I'm carrying the flag
					WaypointNode *targetNode = _WaypointManager->GetClosestWaypointNode(botPos,
						-1, _ABot->GetTeam() == TEAM_RED ? NODE_ITEMFLAG_RED : NODE_ITEMFLAG_BLUE);
					if (targetNode) // Map doesn't have a ITEMFLAG_RED/ITEMFLAG_BLUE node!
						_SetBotTask(new BotTaskGoto(_ABot, targetNode->Pos, true, NODE_SPAWN_RED | NODE_SPAWN_BLUE)); // Don't walk through spawns
				}
			}

			// Free Roaming if still no task
			if (!_HasBotTask())
				_SetBotTask(new BotTaskGoto(_ABot, _WaypointManager->GetRandomWaypointNode(
					_WaypointNodeFlagsProvider->GetInaccessibleNodeFlagsForBot(_ABot))->Pos, false));
		}
	}

	delete closestObjective;
}

void BotBrain::OnSpawn()
{
	_ClearTask();
	_ResetState();
	_ABot->SetSelectedWeapon(WEAPON_PRIMARY);
	_OnSpawn();
}

void BotBrain::OnObjectiveUpdate()
{
	_ClearTask();
}

Bot *BotBrain::_GetBot() const
{
	return _ABot;
}

void BotBrain::_SetBotTask(BotTask *task)
{
	_ClearTask();
	_BotTask = task;
}

bool BotBrain::_HasBotTask() const
{
	return _BotTask != nullptr;
}

void BotBrain::_ClearTask()
{
	delete _BotTask;
	_BotTask = nullptr;
}

void BotBrain::_ResetState()
{
	_States = 0;
	_IsBotDead = false;
	_IsBotInMeleeFight = false;
}

void BotBrain::_AddState(BotState state)
{
	_States |= state;
}

void BotBrain::_RemoveState(BotState state)
{
	_States &= ~state;
}

bool BotBrain::_HasState(BotState state) const
{
	return _States & state;
}