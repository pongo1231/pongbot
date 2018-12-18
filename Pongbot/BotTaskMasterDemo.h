#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterDemo : public BotTaskMaster
{
public:
	BotTaskMasterDemo(Bot *bot);

private:
	void _OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};