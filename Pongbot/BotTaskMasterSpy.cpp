#include "BotTaskMasterSpy.h"

BotTaskMasterSpy::BotTaskMasterSpy(Bot *bot) {
	_UpdateBotTask(new BotTaskCommon(bot));
}

void BotTaskMasterSpy::OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt) {
	int botTaskFlags;
	_BotTaskThink(pressedButtons, movement, lookAt, &botTaskFlags);
}