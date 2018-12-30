#include "BotBehaviour.h"
#include "BotTaskGoto.h"
#include "WaypointManager.h"

Bot *_ABot;
std::queue<BotTask*> _BotTasks;

void BotBehaviour::OnThink()
{
	_OnThink();

	if (!_BotTasks.empty())
	{
		BotTask *task = _BotTasks.front();
		if (task->OnThink(_ABot))
		{
			delete task;
			_BotTasks.pop();
		}
	}
	else
		_BotTasks.push(new BotTaskGoto(_WaypointManager->GetRandomWaypointNode()->Pos, false));
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