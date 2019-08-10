#pragma once
#include "../../IGameFramable.h"
#include <hlsdk/public/mathlib/mathlib.h>
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
	Objective(edict_t* edict, ObjectiveType type, Vector pos, int status) : Edict(edict), Type(type), Pos(pos), Status(status)
	{}

	edict_t* Edict;
	ObjectiveType Type;
	Vector Pos;
	int Status;
};

class ObjectivesProvider : public IGameFramable
{
private:
	ObjectivesProvider()
	{}

public:
	static void Init();
	static void Destroy();

	std::vector<Objective> GetBotPushObjectives(Bot* bot) const;
	std::vector<Objective> GetBotDefendObjectives(Bot* bot) const;
	std::vector<Objective> GetAllObjectives() const;
	int GetRoundTimerStatus() const;

	virtual void OnGameFrame();

private:
	std::vector<Objective> _RedObjectives;
	std::vector<Objective> _BlueObjectives;

	void _UpdateCTFObjectives();
};

extern ObjectivesProvider* _ObjectivesProvider;