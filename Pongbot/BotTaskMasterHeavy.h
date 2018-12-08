#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterHeavy : public BotTaskMaster {
public:
	BotTaskMasterHeavy(Bot *bot);
	~BotTaskMasterHeavy();

public:
	void OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};