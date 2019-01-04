#include "BotBrain.h"
#include "BotTaskGoto.h"
#include "WaypointManager.h"
#include "WaypointNodeFlagsProvider.h"
#include "ObjectivesProvider.h"
#include "CTFFlagStatusType.h"
#include "Util.h"
#include "EntityDataProvider.h"
#include "TFTeam.h"
#include <metamod/ISmmAPI.h>
#include <vector>

#define BEHAVIOUR_UPDATE_TICK 0.2f

extern IVEngineServer *Engine;

Bot *_ABot;
std::queue<BotTask*> _BotTasks;
bool _IsBotDead;
bool _FreeRoaming;
float _DefaultBehaviourUpdateTime;

void BotBrain::OnThink()
{
	if (_ABot->IsDead())
		_IsBotDead = true;
	else
	{
		if (_IsBotDead)
		{
			_IsBotDead = false;
			OnSpawn();
		}

		_OnThink();

		float engineTime = Engine->Time();
		if (_DefaultBehaviourUpdateTime < engineTime)
		{
			_DefaultBehaviour();
			_DefaultBehaviourUpdateTime = engineTime + BEHAVIOUR_UPDATE_TICK;
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

void BotBrain::_DefaultBehaviour()
{
	Vector botPos = _ABot->GetPos();
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

	bool needsNewTask = _BotTasks.empty();
	std::queue<BotTask*> newTaskQueue;

	BotTaskGoto *gotoTask = nullptr;
	if (closestObjective && (_FreeRoaming || needsNewTask))
	{
		if (closestObjective->Type == ITEMFLAG)
		{
			CTFFlagStatusType itemFlagStatus = (CTFFlagStatusType) closestObjective->Status;
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
	if (gotoTask)
		_FreeRoaming = false;
	else if (!gotoTask && needsNewTask)
	{
		_FreeRoaming = true;
		gotoTask = new BotTaskGoto(_ABot, _WaypointManager->GetRandomWaypointNode(
			_WaypointNodeFlagsProvider->GetInaccessibleNodeFlagsForBot(_ABot))->Pos, false);
	}
	if (gotoTask)
		newTaskQueue.push(gotoTask);

	delete closestObjective;

	if (!newTaskQueue.empty())
		SetTaskQueue(newTaskQueue);
}

void BotBrain::OnSpawn()
{
	_ClearTasks();
	_OnSpawn();
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