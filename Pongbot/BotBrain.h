#pragma once
#include "Bot.h"
#include "BotTask.h"
#include "IEventHooker.h"
#include <queue>
#include <vector>

enum BotState
{
// TODO: Signal stuff like needing ammo or so here
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
	bool _IsBotDead;
	bool _IsBotInMeleeFight;

	void _DefaultThink();
	void _ClearTasks();
	void _ResetState();

	virtual void _OnThink() = 0;
	virtual void _OnSpawn() = 0;
};

