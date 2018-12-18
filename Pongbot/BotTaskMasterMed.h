#pragma once
#include "BotTaskMaster.h"

class BotTaskMasterMed : public BotTaskMaster
{
public:
	BotTaskMasterMed(Bot *bot);

private:
	void _OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt);
};