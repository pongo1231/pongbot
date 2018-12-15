#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterPyro : public BotTaskMaster {
public:
	BotTaskMasterPyro(Bot *bot);

private:
	void _OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};