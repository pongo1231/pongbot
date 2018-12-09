#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterSniper : public BotTaskMaster {
public:
	BotTaskMasterSniper(Bot *bot);

public:
	void OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};