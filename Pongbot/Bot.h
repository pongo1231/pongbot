#pragma once
#include "TFClass.h"
#include "WeaponSlot.h"
#include "TFTeam.h"
#include "Player.h"
#include <hlsdk/public/mathlib/mathlib.h>

class BotBrain;
class BotVisibles;
struct TFClassInfo;
class IBotController;
class IPlayerInfo;
struct edict_t;

class Bot
{
public:
	Bot(Player player, const char *name);
	~Bot();

public:
	const char* Name;

	void Think();
	const Player GetPlayer() const;
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
	WeaponSlot GetSelectedWeaponSlot() const;
	void SetSelectedWeapon(WeaponSlot weapon);
	BotVisibles *GetBotVisibles() const;
	bool IsDead() const;
	void ChangeClass(TFClass tfClass);
	void ExecClientCommand(const char *command, ...) const;
	WeaponSlot GetIdealWeaponForRange(float range) const;

private:
	const Player _Player;
	edict_t *_Edict;
	IPlayerInfo *_IPlayerInfo;
	IBotController *_IBotController;
	BotBrain *_BotBrain;
	BotVisibles *_BotVisibles;
	TFClass _CurrentClass;
	QAngle _TargetViewAngle;
	Vector2D _Movement;
	int _PressedButtons;
	TFClassInfo *_ClassInfo;
	WeaponSlot _SelectedWeaponSlot;

	void _SwitchToFittingTeam();
	void _UpdateBotBrain();
	const char *_TFClassToJoinName(TFClass tfClass) const;
	void _RandomClass();
};

