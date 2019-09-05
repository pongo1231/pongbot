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
	inline Player GetPlayer() const
	{
		return _Player;
	}
	inline edict_t* GetEdict() const
	{
		return _Edict;
	}
	inline bool Exists() const
	{
		return _Player.Exists();
	}
	inline Vector GetPos() const
	{
		return _Player.GetPos();
	}
	inline Vector GetEarPos() const
	{
		return _Player.GetHeadPos();
	}
	QAngle GetViewAngle() const;
	void SetViewAngle(QAngle angle);
	inline TFClass GetClass() const
	{
		return _CurrentClass;
	}
	inline TFTeam GetTeam() const
	{
		return _Player.GetTeam();
	}
	void SetMovement(Vector2D movement);
	inline Vector2D GetMovement() const
	{
		return _Movement;
	}
	void SetPressedButtons(int pressedButtons);
	inline int GetPressedButtons() const
	{
		return _PressedButtons;
	}
	inline WeaponSlot GetSelectedWeaponSlot() const
	{
		return _SelectedWeaponSlot;
	}
	void SetSelectedWeapon(WeaponSlot weapon);
	BotVisibles* GetBotVisibles() const;
	inline bool IsDead() const
	{
		return _Player.IsDead();
	}
	void ChangeClass(TFClass tfClass);
	void ExecClientCommand(const char* command, ...) const;
	WeaponSlot GetIdealWeaponForRange(float range) const;
	inline IServerEntity* GetIServerEntity() const
	{
		return _Edict->GetIServerEntity();
	}

private:
	Player _Player;
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

