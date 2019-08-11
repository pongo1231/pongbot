#pragma once
#include "Entity.h"

class IPlayerInfo;

class Player : public Entity
{
public:
	Player() : _IIPlayerInfo(nullptr) {}
	Player(edict_t* edict);
	Player(Entity entity);

public:
	float GetHealth() const;
	float GetFOV() const;
	bool IsSniperZoomedIn() const;
	Vector GetHeadPos() const;
	bool IsReloading() const;
	bool IsDead() const;
	bool Exists() const;
	bool IsBot() const;
	IPlayerInfo* GetPlayerInfo() const;
	QAngle GetAngle() const;
	void SetAngle(QAngle angle);

private:
	IPlayerInfo* _IIPlayerInfo;
};