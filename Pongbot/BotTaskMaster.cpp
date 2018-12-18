#include "BotTaskMaster.h"

Bot *_ABot;
BotTask *_BotTask;

BotTaskMaster::BotTaskMaster(Bot *bot) : _ABot(bot)
{}

BotTaskMaster::~BotTaskMaster()
{
	delete _BotTask;
}

void BotTaskMaster::OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt)
{
	if (!_GetBotTask())
		_UpdateBotTask(new BotTaskCommon(_ABot));

	_BotTask->OnThink(pressedButtons, movement, lookAt);
	_OnThink(pressedButtons, movement, lookAt);
}

Bot *BotTaskMaster::_GetBot() const
{
	return _ABot;
}

BotTask *BotTaskMaster::_GetBotTask() const
{
	return _BotTask;
}

void BotTaskMaster::_UpdateBotTask(BotTask *newTask)
{
	if (newTask) {
		delete _BotTask;
		_BotTask = newTask;
	}
}