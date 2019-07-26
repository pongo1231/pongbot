#pragma once
#include "BotBrain.h"

class BotBrainSniper : public BotBrain
{
public:
	BotBrainSniper(Bot* bot) : BotBrain(bot), _IsBotSniping(false)
	{}

private:
	bool _IsBotSniping;

	virtual void _OnThink();
	virtual void _OnSpawn();
};