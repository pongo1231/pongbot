#pragma once
#include "BotBrain.h"

class BotBrainSniper : public BotBrain
{
public:
	BotBrainSniper(Bot *bot) : BotBrain(bot)
	{}

private:
	bool _IsBotSniping;

	virtual void _OnThink();
	virtual void _OnSpawn();
};