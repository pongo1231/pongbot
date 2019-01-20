#include "Player.h"
#include "EntityDataProvider.h"
#include <metamod/ISmmAPI.h>

extern IServerGameClients *IIServerGameClients;

Player::Player(edict_t *edict) : Entity(edict)
{}

Player::Player(Entity entity) : Entity(entity)
{}

float Player::GetFOV() const
{
	if (!Exists())
		return -1;

	return _EntityDataProvider->GetDataFromEntity<float>(*this, DATA_PLAYER_FOV);
}

bool Player::IsSniperZoomedIn() const
{
	if (!Exists())
		return false;

	return GetFOV() == 20.f;
}

Vector Player::GetHeadPos() const
{
	if (!Exists())
		return Vector();

	Vector headPos;
	IIServerGameClients->ClientEarPosition(GetEdict(), &headPos);

	return headPos;
}

bool Player::IsReloading() const
{
	//return _EntityDataProvider->GetDataFromEntity<edict_t*>(*this, DATA_PLAYER_CURRENTWEAPON);
	return false;
}

bool Player::IsDead() const
{
	return GetHealth() <= 1.f; // Apparently 1 equals to dead
}

bool Player::Exists() const
{
	return Entity::Exists() && IsPlayer();
}