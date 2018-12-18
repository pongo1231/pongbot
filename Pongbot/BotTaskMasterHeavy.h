#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterHeavy : public BotTaskMaster
{
public:
	BotTaskMasterHeavy(Bot *bot);

private:
	void _OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};