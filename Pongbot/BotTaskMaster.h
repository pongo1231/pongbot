#pragma once
#include "Bot.h"
#include "BotTask.h"
#include "BotTaskCollection.h"
#include <hlsdk/public/mathlib/mathlib.h>

class BotTaskMaster
{
public:
	BotTaskMaster(Bot *bot);
	~BotTaskMaster();

public:
	void OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt);

protected:
	Bot *_GetBot() const;
	BotTask *_GetBotTask() const;
	void _UpdateBotTask(BotTask *newTask);

private:
	Bot *_ABot;
	BotTask *_BotTask;

	virtual void _OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt) = 0;
};

