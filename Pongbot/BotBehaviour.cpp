#include "BotBehaviour.h"
#include "BotTaskGoto.h"
#include "WaypointManager.h"
#include "WaypointNodeFlagsProvider.h"

Bot *_ABot;
std::queue<BotTask*> _BotTasks;
bool _IsBotDead;

void BotBehaviour::OnThink()
{
	_OnThink();

	if (_ABot->IsDead())
		_IsBotDead = true;
	else
	{
		if (_IsBotDead)
		{
			_IsBotDead = false;
			OnSpawn();
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
		else
			_BotTasks.push(new BotTaskGoto(_ABot, _WaypointManager->GetRandomWaypointNode(
				_WaypointNodeFlagsProvider->GetInaccessibleNodeFlagsForBot(_ABot))->Pos, false));
	}
}

void BotBehaviour::OnSpawn()
{
	_ClearTasks();
	_OnSpawn();
}

void BotBehaviour::SetTaskQueue(std::queue<BotTask*> taskQueue)
{
	_ClearTasks();
	_BotTasks = taskQueue;
}

Bot *BotBehaviour::_GetBot() const
{
	return _ABot;
}

void BotBehaviour::_ClearTasks()
{
	while (!_BotTasks.empty())
	{
		delete _BotTasks.front();
		_BotTasks.pop();
	}
}