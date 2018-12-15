#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterEngi : public BotTaskMaster {
public:
	BotTaskMasterEngi(Bot *bot);

private:
	void _OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};