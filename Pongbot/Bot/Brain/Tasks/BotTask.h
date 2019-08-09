#pragma once
#include "../../Bot.h"
#include <hlsdk/public/mathlib/vector.h>
#include <hlsdk/game/shared/in_buttons.h>

class BotTask
{
public:
	BotTask(Bot* bot) : _Bot(bot), _IsBotViewAngleOverriden(false), _BotPressedButtons(0), _WeaponSlot(WEAPON_UNKNOWN) {}

public:
	bool OnThink();

protected:
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

	void _ShootAtBadGuys();

	virtual bool _OnThink() = 0; // true == Task done
	virtual void _OnStop() {}
};