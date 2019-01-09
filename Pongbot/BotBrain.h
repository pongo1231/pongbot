#pragma once
#include "Bot.h"
#include "BotTask.h"
#include "IEventHooker.h"
#include <queue>
#include <vector>

class BotBrain : public IEventHooker
{
public:
	BotBrain(Bot *bot);

public:
	void OnThink();
	void OnSpawn();
	void SetTaskQueue(std::queue<BotTask*> taskQueue);

	virtual void OnObjectiveUpdate();

protected:
	Bot *_GetBot() const;

private:
	Bot *_ABot;
	std::queue<BotTask*> _BotTasks;
	bool _IsBotDead;
	float _DefaultBehaviourUpdateTime;
	bool _InMeleeFight;

	void _DefaultBehaviour();
	void _ClearTasks();
	void _ResetState();

	virtual void _OnThink()
	{}
	virtual void _OnSpawn()
	{}
};

