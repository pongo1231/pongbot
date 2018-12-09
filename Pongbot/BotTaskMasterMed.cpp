#include "BotTaskMasterMed.h"

BotTaskMasterMed::BotTaskMasterMed(Bot *bot) {
	_UpdateBotTask(new BotTaskCommon(bot));
}

void BotTaskMasterMed::OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt) {
	int botTaskFlags;
	_BotTaskThink(pressedButtons, movement, lookAt, &botTaskFlags);
}