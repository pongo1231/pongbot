#pragma once
#include "Bot.h"
#include <hlsdk/public/mathlib/vector.h>
#include <hlsdk/game/shared/in_buttons.h>

class BotTask
{
public:
	BotTask(Bot *bot) : _Bot(bot)
	{}

public:
	bool OnThink(); 

protected:
	void _BotMoveTo(Vector pos);
	void _SetBotLookAt(Vector pos);
	void _OverrideBotViewAngle();
	void _AddBotPressedButton(int button);
	Bot *_GetBot() const;

private:
	Bot *_Bot;
	Vector _BotTargetPos;
	Vector _BotTargetLookAt;
	bool _IsBotViewAngleOverriden;
	int _BotPressedButtons;

	void _ShootAtBadGuys();
	void _ChooseBestWeaponForDistance(float distance);

	virtual bool _OnThink() = 0; // true == Task done
};