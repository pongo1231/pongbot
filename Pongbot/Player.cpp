#include "Player.h"
#include "EntityDataProvider.h"
#include <metamod/ISmmAPI.h>

extern IServerGameClients *IIServerGameClients;

float Player::GetFOV() const
{
	return _EntityDataProvider->GetDataFromEdict<float>(GetEdict(), DATA_PLAYER_FOV);
}

bool Player::IsSniperZoomedIn() const
{
	return GetFOV() == 20.f;
}

Vector Player::GetHeadPos() const
{
	Vector headPos;
	IIServerGameClients->ClientEarPosition(GetEdict(), &headPos);

	return headPos;
}

bool Player::IsReloading() const
{
	return _EntityDataProvider->GetDataFromEdict<edict_t*>(GetEdict(), DATA_PLAYER_CURRENTWEAPON);
}