#pragma once
#include "../TF2/Class/TFClass.h"
#include "../TF2/WeaponSlot.h"
#include "../TF2/TFTeam.h"
#include "../TF2/Entity/Player.h"
#include "../TF2/Class/TFClassInfoProvider.h"
#include <hlsdk/public/mathlib/mathlib.h>

class BotBrain;
class BotVisibles;
class IBotController;
class IPlayerInfo;
struct edict_t;

class Bot
{
public:
	Bot(Player player, const char* name);
	~Bot();

public:
	const char* Name;

	void Think();
	const Player GetPlayer() const;
	edict_t* GetEdict() const;
	bool Exists() const;
	Vector GetPos() const;
	Vector GetEarPos() const;
	QAngle GetViewAngle() const;
	void SetViewAngle(QAngle angle);
	TFClass GetClass() const;
	TFTeam GetTeam() const;
	void SetMovement(Vector2D movement);
	Vector2D GetMovement() const;
	void SetPressedButtons(int pressedButtons);
	int GetPressedButtons() const;
	WeaponSlot GetSelectedWeaponSlot() const;
	void SetSelectedWeapon(WeaponSlot weapon);
	BotVisibles* GetBotVisibles() const;
	bool IsDead() const;
	void ChangeClass(TFClass tfClass);
	void ExecClientCommand(const char* command, ...) const;
	WeaponSlot GetIdealWeaponForRange(float range) const;

private:
	const Player _Player;
	edict_t* _Edict;
	IPlayerInfo* _IPlayerInfo;
	IBotController* _IBotController;
	BotBrain* _BotBrain;
	BotVisibles* _BotVisibles;
	TFClass _CurrentClass;
	QAngle _TargetViewAngle;
	Vector2D _Movement;
	int _PressedButtons;
	TFClassInfo _ClassInfo;
	WeaponSlot _SelectedWeaponSlot;

	void _SwitchToFittingTeam();
	void _UpdateBotBrain();
	const char* _TFClassToJoinName(TFClass tfClass) const;
	void _RandomClass();
};

