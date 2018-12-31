#include "BotBehaviour.h"
#include "BotTaskGoto.h"
#include "WaypointManager.h"

Bot *_ABot;
std::queue<BotTask*> _BotTasks;
bool _IsBotDead = false;

void BotBehaviour::OnThink()
{
	_OnThink();

	if (_ABot->IsDead())
	{
		_IsBotDead = true;
	}
	else
	{
		if (_IsBotDead)
		{
			OnSpawn();
			_IsBotDead = false;
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
			_BotTasks.push(new BotTaskGoto(_ABot, _WaypointManager->GetRandomWaypointNode()->Pos, false));
	}
}

void BotBehaviour::OnSpawn()
{
	_OnSpawn();

	_BotTasks = std::queue<BotTask*>();
}

void BotBehaviour::SetTaskQueue(std::queue<BotTask*> taskQueue)
{
	// Clean up first
	while (!_BotTasks.empty())
	{
		delete _BotTasks.front();
		_BotTasks.pop();
	}

	_BotTasks = taskQueue;
}

Bot *BotBehaviour::_GetBot() const
{
	return _ABot;
}