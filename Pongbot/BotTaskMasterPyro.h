#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterPyro : public BotTaskMaster {
public:
	BotTaskMasterPyro(Bot *bot);
	~BotTaskMasterPyro();

public:
	void OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};