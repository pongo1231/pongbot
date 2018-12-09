#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterEngi : public BotTaskMaster {
public:
	BotTaskMasterEngi(Bot *bot);

public:
	void OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};