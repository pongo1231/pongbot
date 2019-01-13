#pragma once
#include "TFTeam.h"
#include <hlsdk/public/edict.h>

class EntityInfo
{
public:
	EntityInfo(edict_t *edict) : _MEdict(edict)
	{}

public:
	float GetHealth() const;
	TFTeam GetTeam() const;
	Vector GetPos() const;
	bool IsPlayer() const;

protected:
	edict_t *_GetEdict() const;

private:
	edict_t *_MEdict;
};