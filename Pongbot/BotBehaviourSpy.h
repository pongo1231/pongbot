#pragma once
#include "BotBehaviour.h"

class BotBehaviourSpy : public BotBehaviour
{
public:
	BotBehaviourSpy(Bot *bot) : BotBehaviour(bot)
	{}

private:
	virtual void _OnThink();
};