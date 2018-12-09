#include "BotTaskMasterHeavy.h"

BotTaskMasterHeavy::BotTaskMasterHeavy(Bot *bot) {
	_UpdateBotTask(new BotTaskCommon(bot));
}

void BotTaskMasterHeavy::OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt) {
	int botTaskFlags;
	_BotTaskThink(pressedButtons, movement, lookAt, &botTaskFlags);
}