#include "BotTaskMaster.h"

BotTask *_BotTask;

BotTaskMaster::~BotTaskMaster() {
	delete _BotTask;
}

BotTask *BotTaskMaster::_GetBotTask() const {
	return _BotTask;
}

void BotTaskMaster::_UpdateBotTask(BotTask *newTask) {
	if (newTask) {
		delete _BotTask;
		_BotTask = newTask;
	}
}

void BotTaskMaster::_BotTaskThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt, int *taskFlags) {
	_BotTask->OnThink(pressedButtons, movement, lookAt, taskFlags);
}