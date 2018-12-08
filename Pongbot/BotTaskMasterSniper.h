#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterSniper : public BotTaskMaster {
public:
	BotTaskMasterSniper(Bot *bot);
	~BotTaskMasterSniper();

public:
	void OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};