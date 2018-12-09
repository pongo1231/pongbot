#include "BotTask.h"

Bot *_Bot;

BotTask::BotTask(Bot *bot) : _Bot(bot) {}

Bot *BotTask::_GetBot() const {
	return _Bot;
}