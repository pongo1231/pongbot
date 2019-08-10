#include <stdafx.h>
#include "BotBrain.h"
#include "Tasks/BotTaskGoto.h"
#include "Tasks/BotTaskAggressiveCombat.h"
#include "../../Waypoint/WaypointManager.h"
#include "../../Waypoint/WaypointNodeFlagsProvider.h"
#include "../../TF2/Objectives/ObjectivesProvider.h"
#include "../../TF2/Objectives/CTFFlagStatusType.h"
#include "../../Util.h"
#include "../Visibles/BotVisibles.h"
#include "../../ConVarHolder.h"
#include "../../TF2/WeaponSlot.h"
#include "../../TF2/Entity/CTFFlag.h"
#include <stdint.h> // uint8_t for Linux

extern IVEngineServer* Engine;

void BotBrain::OnThink()
{
	if (_GetBot()->IsDead())
	{
		_IsBotDead = true;
	}
	else
	{
		if (_IsBotDead)
		{
			OnSpawn();
		}

		float engineTime = Engine->Time();
		if (_ThinkTime < engineTime)
		{
			_ThinkTime = engineTime + _ConVarHolder->CVarBotBrainThinkTick->GetFloat();
			_OnThink();
			_DefaultThink();
		}

		if (_HasBotTask() && _BotTask->OnThink())
		{
			_ClearTask();
		}
	}
}

void BotBrain::_DefaultThink()
{
	Bot* bot = _GetBot();
	Vector botPos = _ABot->GetPos();

	/* Tasks which should be able to override current ones */

	// Melee combat
	BotVisibleTarget currentTarget = bot->GetBotVisibles()->GetMostImportantTarget();
	if (currentTarget.IsValid() && bot->GetSelectedWeaponSlot() == WeaponSlot::WEAPON_MELEE)
	{
		if (!_IsBotInMeleeFight)
		{
			_IsBotInMeleeFight = true;
			_SetBotTask(new BotTaskAggressiveCombat(bot, currentTarget.GetEntity(), WeaponSlot::WEAPON_MELEE));
		}
	}
	else
	{
		_IsBotInMeleeFight = false;
	}

	/* Filler Tasks in case the bot has nothing to do */

	if (!_HasBotTask())
	{
		/* Check for objectives */

		std::vector<Objective> pushObjectives = _ObjectivesProvider->GetBotPushObjectives(bot);
		// Get closest objective
		bool foundObjective = false;
		Objective closestObjective(nullptr, ObjectiveType::UNK, Vector(), 0); // Placeholder
		float closestObjectiveDistance = 99999.f;
		for (Objective pushObjective : pushObjectives)
		{
			float distance = botPos.DistTo(pushObjective.Pos);
			if (distance < closestObjectiveDistance)
			{
				foundObjective = true;
				closestObjective = pushObjective;
				closestObjectiveDistance = distance;
			}
		}

		if (foundObjective)
		{
			// CTF Flag stuff
			if (closestObjective.Type == ITEMFLAG)
			{
				CTFFlagStatusType itemFlagStatus = (CTFFlagStatusType) closestObjective.Status;
				if (itemFlagStatus == CTFFlagStatusType::CTF_UNTOUCHED || itemFlagStatus == CTFFlagStatusType::CTF_DROPPED) // The flag should be picked up
				{
					_SetBotTask(new BotTaskGoto(bot, closestObjective.Pos, false));
				}
				else if (CTFFlag(closestObjective.Edict).GetOwner() == bot->GetEdict()->m_iIndex)
				{
					// I'm carrying the flag
					WaypointNode* targetNode = _WaypointManager->GetClosestWaypointNode(botPos, -1, bot->GetTeam() == TEAM_RED ? NODE_ITEMFLAG_RED : NODE_ITEMFLAG_BLUE);
					if (targetNode) // Map doesn't have a ITEMFLAG_RED/ITEMFLAG_BLUE node!
					{
						_SetBotTask(new BotTaskGoto(bot, targetNode->Pos, true, NODE_SPAWN_RED | NODE_SPAWN_BLUE)); // Don't walk through spawns
					}
				}
			}

			// Free Roam if still no task
			if (!_HasBotTask())
			{
				_SetBotTask(new BotTaskGoto(bot, _WaypointManager->GetRandomWaypointNode(
					_WaypointNodeFlagsProvider->GetInaccessibleNodeFlagsForBot(_ABot))->Pos, false));
			}
		}
	}
}

void BotBrain::OnSpawn()
{
	_ClearTask();
	_ResetState();
	_GetBot()->SetSelectedWeapon(WEAPON_PRIMARY);
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

void BotBrain::_SetBotTask(BotTask* task)
{
	_ClearTask();
	_BotTask = task;
}

bool BotBrain::_HasBotTask() const
{
	return _BotTask;
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