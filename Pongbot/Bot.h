#pragma once
#include <hlsdk/public/edict.h>
#include <hlsdk/public/game/server/iplayerinfo.h>

class BotHelper;
class WaypointNode;
class BotBehaviour;
class BotVisibles;
enum TFClass;
enum TFTeam;

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
	BotVisibles *GetBotVisibles() const;
	bool IsDead() const;
	void ChangeClass(TFClass tfClass);
	void ExecClientCommand(const char *command) const;

private:
	edict_t *_Edict;
	IBotController *_IIBotController;
	IPlayerInfo *_IIPlayerInfo;
	BotBehaviour *_BotBehaviour;
	BotVisibles *_BotVisibles;
	TFClass _CurrentClass;
	QAngle _TargetViewAngle;
	Vector2D _Movement;
	int _PressedButtons;

	void _UpdateBotBehaviour();
	void _TFClassToJoinName(TFClass tfClass, char *tfClassName);
	void _RandomClass();
};

