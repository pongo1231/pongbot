#pragma once
#include "Bot.h"
#include "BotTask.h"
#include <hlsdk/public/mathlib/mathlib.h>
#include <queue>

class BotBehaviour
{
public:
	BotBehaviour(Bot *bot) : _ABot(bot), _IsBotDead(false), _FreeRoaming(false)
	{}

public:
	void OnThink();
	void OnSpawn();
	void SetTaskQueue(std::queue<BotTask*> taskQueue);

protected:
	Bot *_GetBot() const;

private:
	Bot *_ABot;
	std::queue<BotTask*> _BotTasks;
	bool _IsBotDead;
	bool _FreeRoaming;
	float _DefaultBehaviourUpdateTime;

	void _DefaultBehaviour();
	void _ClearTasks();

	virtual void _OnThink()
	{}
	virtual void _OnSpawn()
	{}
};
