#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterMed : public BotTaskMaster {
public:
	BotTaskMasterMed(Bot *bot);

public:
	void OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};