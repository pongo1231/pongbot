#pragma once
#include "BotBehaviour.h"

class BotBehaviourHeavy : public BotBehaviour
{
public:
	BotBehaviourHeavy(Bot *bot) : BotBehaviour(bot)
	{}

private:
	virtual void _OnThink();
};