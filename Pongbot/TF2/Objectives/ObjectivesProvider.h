#pragma once
#include "../../IGameFramable.h"
#include "../Events/IEventHooker.h"
#include "../Entity/RoundTimer.h"
#include <hlsdk/public/edict.h>
#include <vector>
#include <map>

class Bot;

enum ObjectiveType
{
	UNK = -1,
	ITEMFLAG,
	CAPTURE_POINT
};

struct Objective
{
public:
	Objective(edict_t* edict, ObjectiveType type, Vector pos, int status) : Edict(edict), _Type(type), _Pos(pos), _Status(status) {}
	int operator=(const Objective& target)
	{
		Edict = target.Edict;
		_Type = target.GetType();
		_Pos = target.GetPos();
		_Status = target.GetStatus();
	}

public:
	edict_t* Edict;

	ObjectiveType GetType() const
	{
		return _Type;
	}

	Vector GetPos() const
	{
		return _Pos;
	}

	int GetStatus() const
	{
		return _Status;
	}

private:
	ObjectiveType _Type;
	Vector _Pos;
	int _Status;
};

class ObjectivesProvider : public IGameFramable, IEventHooker
{
private:
	ObjectivesProvider() {}

public:
	static void Init();
	static void Destroy();

	std::vector<Objective> GetBotPushObjectives(Bot* bot) const;
	std::vector<Objective> GetBotDefendObjectives(Bot* bot) const;
	std::vector<Objective> GetAllObjectives() const;
	int GetRoundTimerStatus() const;

	virtual void OnGameFrame();
	virtual void OnLevelInit(const char* pMapName, char const* pMapEntities,
		char const* pOldLevel, char const* pLandmarkName, bool loadGame, bool background);

private:
	std::vector<Objective> _RedObjectives;
	std::vector<Objective> _BlueObjectives;
	RoundTimer _RoundTimer;

	void _UpdateCTFObjectives();
	void _UpdateRoundTimer();
};

extern ObjectivesProvider* _ObjectivesProvider;