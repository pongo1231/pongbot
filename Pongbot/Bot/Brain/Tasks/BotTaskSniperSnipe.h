#pragma once
#include "BotTask.h"

extern IVEngineServer* Engine;

class BotTaskSniperSnipe : public BotTask
{
public:
	BotTaskSniperSnipe(Bot *bot, int campTime = -1, QAngle prefAngle = {0.f, 0.f, 0.f}) : BotTask(bot),
		_CampTime(Engine->Time() + campTime), _PrefAngle(prefAngle), _ShootTime(0.f)
	{
		_DoStuckPosPanicHandling = false;
	}

private:
	int _CampTime;
	QAngle _PrefAngle;
	float _ShootTime;

	virtual bool _OnThink();
	virtual void _OnStop();
};