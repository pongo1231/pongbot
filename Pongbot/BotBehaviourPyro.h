#pragma once
#include "BotBehaviour.h"

class BotBehaviourPyro : public BotBehaviour
{
public:
	BotBehaviourPyro(Bot *bot) : BotBehaviour(bot)
	{}

private:
	virtual void _OnThink();
};