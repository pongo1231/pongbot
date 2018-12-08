#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterSpy : public BotTaskMaster {
public:
	BotTaskMasterSpy(Bot *bot);
	~BotTaskMasterSpy();

public:
	void OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};