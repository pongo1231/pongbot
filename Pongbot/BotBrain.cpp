#include "BotBrain.h"
#include "BotTaskGoto.h"
#include "BotTaskAggressiveCombat.h"
#include "WaypointManager.h"
#include "WaypointNodeFlagsProvider.h"
#include "ObjectivesProvider.h"
#include "CTFFlagStatusType.h"
#include "Util.h"
#include "EntityDataProvider.h"
#include "BotVisibles.h"
#include "ConVarHolder.h"
#include "WeaponSlot.h"
#include <metamod/ISmmAPI.h>
#include <vector>
#include <stdint.h> // uint8_t for Linux

extern IVEngineServer *Engine;

Bot *_ABot;
std::queue<BotTask*> _BotTasks;
float _ThinkTime;
unsigned int _States;

void BotBrain::OnThink()
{
	if (_ABot->IsDead())
		_AddState(BOTSTATE_DEAD);
	else
	{
		if (_States & BOTSTATE_DEAD)
		{
			_RemoveState(BOTSTATE_DEAD);
			OnSpawn();
		}

		float engineTime = Engine->Time();
		if (_ThinkTime < engineTime)
		{
			_ThinkTime = engineTime + _ConVarHolder->CVarBotBrainThinkTick->GetFloat();
			_OnThink();
			_DefaultThink();
		}

		if (!_BotTasks.empty())
		{
			BotTask *task = _BotTasks.front();
			if (task->OnThink())
			{
				delete task;
				_BotTasks.pop();
			}
		}
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

	std::queue<BotTask*> newTaskQueue;

	// Melee combat
	BotVisibleTarget *currentTarget = _ABot->GetBotVisibles()->GetMostImportantTarget();
	if (currentTarget && _ABot->GetSelectedWeaponSlot() == WEAPON_MELEE && !_HasState(BOTSTATE_MELEEFIGHT))
	{
		_AddState(BOTSTATE_MELEEFIGHT);
		newTaskQueue.push(new BotTaskAggressiveCombat(_ABot, currentTarget->Edict, WEAPON_MELEE));
	}
	else if (_HasState(BOTSTATE_MELEEFIGHT))
		_RemoveState(BOTSTATE_MELEEFIGHT);

	/* Filler Tasks in case the bot has nothing to do */

	if (_BotTasks.empty())
	{
		if (newTaskQueue.empty())
		{
			BotTaskGoto *gotoTask = nullptr;
			if (closestObjective)
			{
				// CTF Flag stuff
				if (closestObjective->Type == ITEMFLAG)
				{
					CTFFlagStatusType itemFlagStatus = (CTFFlagStatusType)closestObjective->Status;
					if (itemFlagStatus == CTF_UNTOUCHED || itemFlagStatus == CTF_DROPPED) // The flag should be picked up
						gotoTask = new BotTaskGoto(_ABot, closestObjective->Pos, false);
					else if (_EntityDataProvider->GetDataFromEdict<int>(closestObjective->Edict, DATA_FLAG_OWNER)
						== Engine->IndexOfEdict(_ABot->GetEdict()))
					{
						// I'm carrying the flag
						WaypointNode *targetNode = _WaypointManager->GetClosestWaypointNode(botPos,
							-1, _ABot->GetTeam() == TEAM_RED ? NODE_ITEMFLAG_RED : NODE_ITEMFLAG_BLUE);
						if (targetNode) // Map doesn't have a ITEMFLAG_RED/ITEMFLAG_BLUE node!
							gotoTask = new BotTaskGoto(_ABot, targetNode->Pos, true, NODE_SPAWN_RED | NODE_SPAWN_BLUE); // Don't walk through spawns
					}
				}
			}

			// Free Roaming
			if (!gotoTask)
				gotoTask = new BotTaskGoto(_ABot, _WaypointManager->GetRandomWaypointNode(
					_WaypointNodeFlagsProvider->GetInaccessibleNodeFlagsForBot(_ABot))->Pos, false);

			newTaskQueue.push(gotoTask);
		}
	}

	delete closestObjective;

	if (!newTaskQueue.empty())
		SetTaskQueue(newTaskQueue);
}

void BotBrain::OnSpawn()
{
	_ClearTasks();
	_ABot->SetSelectedWeapon(WEAPON_PRIMARY);
	_OnSpawn();
}

void BotBrain::OnObjectiveUpdate()
{
	_ClearTasks();
}

void BotBrain::SetTaskQueue(std::queue<BotTask*> taskQueue)
{
	_ClearTasks();
	_BotTasks = taskQueue;
}

Bot *BotBrain::_GetBot() const
{
	return _ABot;
}

void BotBrain::_ClearTasks()
{
	while (!_BotTasks.empty())
	{
		delete _BotTasks.front();
		_BotTasks.pop();
	}
}

void BotBrain::_ResetState()
{
	_States = 0;
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