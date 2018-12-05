#include "BotTaskHandler.h"
#include "Bot.h"
#include "BotTask.h"
#include "BotTaskCollection.h"
#include "Util.h"

Bot *__Bot;
BotTask *_BotTask;

BotTaskHandler::BotTaskHandler(Bot *bot) : __Bot(bot) {
	_BotTask = new BotTaskRoamAround(bot);
}

BotTaskHandler::~BotTaskHandler() {
	delete _BotTask;
}

void BotTaskHandler::OnTick(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt) {
	if (_BotTask)
		_BotTask->OnTick(pressedButtons, movement, lookAt);

	if (!movement)
		movement = new Vector2D();
	if (!lookAt)
		lookAt = new QAngle();
}