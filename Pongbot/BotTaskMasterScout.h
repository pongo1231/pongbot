#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterScout : public BotTaskMaster {
public:
	BotTaskMasterScout(Bot *bot);
	~BotTaskMasterScout();

public:
	void OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};