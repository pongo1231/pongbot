#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterSniper : public BotTaskMaster {
public:
	BotTaskMasterSniper(Bot *bot);

private:
	void _OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};