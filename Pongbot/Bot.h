#pragma once
#include "TFTeam.h"
#include "TFClass.h"
#include "WeaponSlot.h"
#include <hlsdk/public/edict.h>
#include <hlsdk/public/game/server/iplayerinfo.h>

class BotHelper;
class WaypointNode;
class BotBrain;
class BotVisibles;

class Bot
{
public:
	Bot(edict_t *edict, const char *name);
	~Bot();

public:
	const char *Name;

	void Think();
	edict_t *GetEdict() const;
	bool Exists() const;
	Vector GetPos() const;
	Vector GetEarPos() const;
	QAngle GetViewAngle() const;
	void SetViewAngle(QAngle angle);
	TFClass GetClass() const;
	TFTeam GetTeam() const;
	void SetMovement(Vector2D movement);
	void SetPressedButtons(int pressedButtons);
	void SetSelectedWeapon(WeaponSlot weapon);
	BotVisibles *GetBotVisibles() const;
	bool IsDead() const;
	void ChangeClass(TFClass tfClass);
	void ExecClientCommand(const char *command, ...) const;

private:
	edict_t *_Edict;
	IBotController *_IIBotController;
	IPlayerInfo *_IIPlayerInfo;
	BotBrain *_BotBrain;
	BotVisibles *_BotVisibles;
	TFClass _CurrentClass;
	QAngle _TargetViewAngle;
	Vector2D _Movement;
	int _PressedButtons;
	WeaponSlot _SelectedWeapon;

	void _SwitchToFittingTeam();
	void _UpdateBotBrain();
	void _TFClassToJoinName(TFClass tfClass, char *tfClassName);
	void _RandomClass();
};

