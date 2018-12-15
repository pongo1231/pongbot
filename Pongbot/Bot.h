#pragma once
#include <hlsdk/public/edict.h>
#include <hlsdk/public/game/server/iplayerinfo.h>

class BotHelper;
class BotTaskMaster;
class BotVisibles;
class WaypointNode;
enum TFClass;
enum TFTeam;

class Bot {
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
	QAngle GetAngle() const;
	TFClass GetClass() const;
	TFTeam GetTeam() const;
	BotVisibles *GetBotVisibles() const;
	bool IsDead() const;
	void ChangeClass(TFClass tfClass);

private:
	edict_t *_Edict;
	IBotController *_IIBotController;
	IPlayerInfo *_IIPlayerInfo;
	BotTaskMaster *_BotTaskMaster;
	BotVisibles *_BotVisibles;

	TFClass _CurrentClass;

	void _TFClassToJoinName(TFClass tfClass, char *tfClassName);
	void _RandomClass();
};

