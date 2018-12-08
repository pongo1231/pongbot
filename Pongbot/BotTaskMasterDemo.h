#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterDemo : public BotTaskMaster {
public:
	BotTaskMasterDemo(Bot *bot);

public:
	void OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};