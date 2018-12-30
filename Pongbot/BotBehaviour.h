#pragma once
#include "Bot.h"
#include "BotTask.h"
#include <hlsdk/public/mathlib/mathlib.h>
#include <queue>

class BotBehaviour
{
public:
	BotBehaviour(Bot *bot) : _ABot(bot)
	{}

public:
	void OnThink();
	void SetTaskQueue(std::queue<BotTask*> taskQueue);

protected:
	Bot *_GetBot() const;

private:
	Bot *_ABot;
	std::queue<BotTask*> _BotTasks;

	virtual void _OnThink()
	{}
	virtual void _OnSpawn()
	{}
	virtual void _OnFlagPickup()
	{}
};

