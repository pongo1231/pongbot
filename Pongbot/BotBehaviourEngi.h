#pragma once
#include "BotBehaviour.h"

class BotBehaviourEngi : public BotBehaviour
{
public:
	BotBehaviourEngi(Bot *bot) : BotBehaviour(bot)
	{}

private:
	virtual void _OnThink();
};