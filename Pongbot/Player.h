#pragma once
#include "Entity.h"

class Player : public Entity
{
public:
	Player(edict_t *edict);
	Player(Entity entity);

public:
	float GetFOV() const;
	bool IsSniperZoomedIn() const;
	Vector GetHeadPos() const;
	bool IsReloading() const;
	bool IsDead() const;
	bool Exists() const;
};