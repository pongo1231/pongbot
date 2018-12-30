#pragma once
#include "BotBehaviour.h"

class BotBehaviourDemo : public BotBehaviour
{
public:
	BotBehaviourDemo(Bot *bot) : BotBehaviour(bot)
	{}

private:
	virtual void _OnThink();
};