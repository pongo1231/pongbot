#include "BotTaskMaster.h"

Bot *_ABot;
BotTask *_BotTask;
BotTask *_BotTaskFallback;

BotTaskMaster::BotTaskMaster(Bot *bot) : _ABot(bot), _BotTaskFallback(new BotTaskCommon(bot))
{}

BotTaskMaster::~BotTaskMaster()
{
	delete _BotTask;
	delete _BotTaskFallback;
}

void BotTaskMaster::OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt)
{
	_OnThink(pressedButtons, movement, lookAt);

	if (_BotTask)
		_BotTask->OnThink(pressedButtons, movement, lookAt);

	// Do common stuff in case movement or lookAt is null
	_BotTaskFallback->OnThink(pressedButtons, movement, lookAt);
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