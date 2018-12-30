#pragma once
#include <hlsdk/public/mathlib/mathlib.h>
#include <hlsdk/game/shared/in_buttons.h>

class Bot;
enum TFTeam;

class BotTask
{
public:
	bool OnThink(Bot *bot); 

protected:
	void _BotMoveTo(Vector pos);
	void _SetBotLookAt(Vector pos);
	void _OverrideBotViewAngle();
	void _AddBotPressedButton(int button);
	Vector _GetBotPos() const;
	Vector _GetBotEarPos() const;
	TFTeam _GetBotTeam() const;

private:
	Vector _BotCurrentPos;
	Vector _BotEarPos;
	TFTeam _BotTeam;

	Vector _BotTargetPos;
	Vector _BotTargetLookAt;
	bool _IsBotViewAngleOverriden;
	int _BotPressedButtons;

	virtual bool _OnThink() = 0; // true == Task done
};