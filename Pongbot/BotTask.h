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
	QAngle _SetBotViewAngle(QAngle angle);
	void _OverrideBotViewAngle(bool override);
	void _AddBotPressedButton(int button);
	Vector _GetBotPos() const;
	TFTeam _GetBotTeam() const;

private:
	Vector _BotCurrentPos;
	Vector _BotTargetPos;
	QAngle _BotViewAngle;
	bool _IsBotViewAngleOverriden;
	int _BotPressedButtons;
	TFTeam _BotTeam;

	virtual bool _OnThink() = 0; // true == Task done
};