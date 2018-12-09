#include "BotTaskMasterScout.h"

BotTaskMasterScout::BotTaskMasterScout(Bot *bot) {
	_UpdateBotTask(new BotTaskCommon(bot));
}

void BotTaskMasterScout::OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt) {
	int botTaskFlags;
	_BotTaskThink(pressedButtons, movement, lookAt, &botTaskFlags);
}