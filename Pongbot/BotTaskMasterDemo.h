#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterDemo : public BotTaskMaster {
public:
	BotTaskMasterDemo(Bot *bot);
	~BotTaskMasterDemo();

public:
	void OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};