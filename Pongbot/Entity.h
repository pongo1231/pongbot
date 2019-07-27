#pragma once
#include "TFTeam.h"

struct edict_t;
class Vector;

class Entity
{
public:
	Entity(edict_t* edict) : _MEdict(edict) {}
	bool operator ==(Entity other)
	{
		return Exists() && GetEdict() == other.GetEdict();
	}

public:
	edict_t* GetEdict() const;
	float GetHealth() const;
	TFTeam GetTeam() const;
	Vector GetPos() const;
	bool IsPlayer() const;
	bool Exists() const;
	const char* GetEdictClassName() const;

private:
	edict_t* _MEdict;
};