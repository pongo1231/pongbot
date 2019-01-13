#pragma once
#include "EntityInfo.h"

class PlayerInfo : public EntityInfo
{
public:
	PlayerInfo(edict_t *edict) : EntityInfo(edict)
	{}

public:
	float GetFOV() const;
	bool IsSniperZoomedIn() const;
	Vector GetHeadPos() const;
};