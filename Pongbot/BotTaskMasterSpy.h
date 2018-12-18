#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterSpy : public BotTaskMaster
{
public:
	BotTaskMasterSpy(Bot *bot);

private:
	void _OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};