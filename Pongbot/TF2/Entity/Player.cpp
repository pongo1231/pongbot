#include "stdafx.h"
#include "Player.h"
#include "EntityDataProvider.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/game/server/iplayerinfo.h>

extern IServerGameClients *IIServerGameClients;
extern IPlayerInfoManager *IIPlayerInfoManager;

Player::Player(edict_t* edict) : Entity(edict), _IIPlayerInfo(IIPlayerInfoManager->GetPlayerInfo(edict)) {}

Player::Player(Entity entity) : Entity(entity), _IIPlayerInfo(IIPlayerInfoManager->GetPlayerInfo(entity.GetEdict())) {}

float Player::GetHealth() const
{
	if (!Exists())
	{
		return -1;
	}

	return _IIPlayerInfo->GetHealth();
}

float Player::GetFOV() const
{
	if (!Exists())
	{
		return -1;
	}

	return _EntityDataProvider->GetDataFromEntity<float>(*this, DATA_PLAYER_FOV);
}

bool Player::IsSniperZoomedIn() const
{
	if (!Exists())
	{
		return false;
	}

	return GetFOV() == 20.f;
}

Vector Player::GetHeadPos() const
{
	if (!Exists())
	{
		return Vector();
	}

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
	return _IIPlayerInfo->IsDead();
}

bool Player::Exists() const
{
	return Entity::Exists() && IsPlayer();
}