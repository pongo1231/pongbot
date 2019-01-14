#pragma once
#include "TFTeam.h"
#include <hlsdk/public/edict.h>

class Entity
{
public:
	Entity(edict_t *edict) : _MEdict(edict)
	{}
	bool operator==(const Entity &other)
	{
		return GetEdict() == other.GetEdict();
	}
	bool operator!=(const Entity &other)
	{
		return GetEdict() != other.GetEdict();
	}

public:
	edict_t *GetEdict() const;
	float GetHealth() const;
	TFTeam GetTeam() const;
	Vector GetPos() const;
	bool IsPlayer() const;
	bool Exists() const;

private:
	edict_t *_MEdict;
};