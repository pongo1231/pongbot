#pragma once
#include "Bot.h"
#include "BotTask.h"
#include "BotTaskCollection.h"
#include <hlsdk/public/mathlib/mathlib.h>

class BotTaskMaster {
public:
	~BotTaskMaster();

public:
	virtual void OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt) = 0;

protected:
	BotTask *_GetBotTask() const;
	void _UpdateBotTask(BotTask *newTask);
	void _BotTaskThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt, int *taskFlags);

private:
	BotTask *_BotTask;
};

