#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterScout : public BotTaskMaster
{
public:
	BotTaskMasterScout(Bot *bot);

private:
	void _OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};