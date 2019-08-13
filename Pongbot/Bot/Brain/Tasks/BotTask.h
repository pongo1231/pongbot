#pragma once
#include "../../Bot.h"
#include <hlsdk/public/mathlib/vector.h>
#include <hlsdk/game/shared/in_buttons.h>

class BotTask
{
public:
	BotTask(Bot* bot) : _Bot(bot), _IsBotViewAngleOverriden(false), _BotPressedButtons(0),
		_WeaponSlot(WEAPON_UNKNOWN), _LastPos(0.f, 0.f, 0.f), _PosStuckTime(0), _PosStuckTries(0),
		_AbortTask(false), _DoStuckPosPanicHandling(true) {}

public:
	bool OnThink();

protected:
	bool _AbortTask;
	bool _DoStuckPosPanicHandling;

	void _BotMoveTo(Vector pos);
	void _SetBotLookAt(Vector pos);
	void _OverrideBotViewAngle();
	void _AddBotPressedButton(int button);
	void _SetBotWeaponSlot(WeaponSlot weaponSlot);
	Bot* _GetBot() const;

private:
	Bot* _Bot;
	Vector _BotTargetPos;
	Vector _BotTargetLookAt;
	bool _IsBotViewAngleOverriden;
	int _BotPressedButtons;
	WeaponSlot _WeaponSlot;
	Vector _LastPos;
	int _PosStuckTime;
	int _PosStuckTries;

	void _ShootAtBadGuys();
	void _CheckIfStuckInPos();

	virtual bool _OnThink() = 0; // true == Task done
	virtual void _OnStop() {}
	virtual void _OnBotStuckPanic() {}
	virtual void _OnBotDefinitelyStuck() {}
};