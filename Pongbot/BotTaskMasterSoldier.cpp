#include "BotTaskMasterSoldier.h"

BotTaskMasterSoldier::BotTaskMasterSoldier(Bot *bot) {
	_UpdateBotTask(new BotTaskCommon(bot));
}

void BotTaskMasterSoldier::OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt) {
	int botTaskFlags;
	_BotTaskThink(pressedButtons, movement, lookAt, &botTaskFlags);
}