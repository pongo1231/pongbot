#include "BotTaskHandler.h"
#include "Bot.h"
#include "BotTask.h"
#include "BotTaskCollection.h"
#include "Util.h"

Bot *_ABot;
BotTask *_BotTask;

BotTaskHandler::BotTaskHandler(Bot *bot) : _ABot(bot) {
	_BotTask = new BotTaskRoamAround(bot);
}

BotTaskHandler::~BotTaskHandler() {
	delete _BotTask;
}

void BotTaskHandler::OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt) {
	if (_BotTask)
		_BotTask->OnThink(pressedButtons, movement, lookAt);

	if (!movement)
		movement = new Vector2D();
	if (!lookAt)
		lookAt = new QAngle();
}