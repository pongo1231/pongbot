#pragma once
#include "BotBehaviour.h"

class BotBehaviourScout : public BotBehaviour
{
public:
	BotBehaviourScout(Bot *bot) : BotBehaviour(bot)
	{}

private:
	virtual void _OnThink();
};