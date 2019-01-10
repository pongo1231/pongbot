#pragma once
#include "Bot.h"
#include "BotTask.h"
#include "IEventHooker.h"
#include <queue>
#include <vector>

enum BotState
{
	BOTSTATE_DEAD = 1,
	BOTSTATE_MELEEFIGHT = 2,
	BOTSTATE_PYRO_FLAMETHROWERRUSH = 4
};

class BotBrain : public IEventHooker
{
public:
	BotBrain(Bot *bot) : _ABot(bot)
	{}

public:
	void OnThink();
	void OnSpawn();
	void SetTaskQueue(std::queue<BotTask*> taskQueue);

	virtual void OnObjectiveUpdate();

protected:
	Bot *_GetBot() const;
	void _AddState(BotState state);
	void _RemoveState(BotState state);
	bool _HasState(BotState state) const;

private:
	Bot *_ABot;
	std::queue<BotTask*> _BotTasks;
	float _ThinkTime;
	unsigned int _States;

	void _DefaultThink();
	void _ClearTasks();
	void _ResetState();

	virtual void _OnThink() = 0;
	virtual void _OnSpawn()
	{}
};

