#pragma once
#include "Entity.h"

class IPlayerInfo;

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
	bool IsConnected() const;
	bool IsDead() const;
	bool Exists() const;

private:
	IPlayerInfo *_IIPlayerInfo;
	bool _IsPlayerInvalid;
};