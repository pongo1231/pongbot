#pragma once
#include "Entity.h"

class Player : public Entity
{
public:
	Player(edict_t *edict) : Entity(edict)
	{}
	Player(Entity entity) : Entity(entity.GetEdict())
	{}

public:
	float GetFOV() const;
	bool IsSniperZoomedIn() const;
	Vector GetHeadPos() const;
	bool IsReloading() const;
};