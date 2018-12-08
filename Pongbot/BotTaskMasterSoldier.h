#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterSoldier : public BotTaskMaster {
public:
	BotTaskMasterSoldier(Bot *bot);
	~BotTaskMasterSoldier();

public:
	void OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};