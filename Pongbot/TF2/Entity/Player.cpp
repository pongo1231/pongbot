#include "stdafx.h"
#include "Player.h"
#include "EntityDataProvider.h"

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
		return Vector(0.f, 0.f, 0.f);
	}

	Vector headPos;
	IIServerGameClients->ClientEarPosition(GetEdict(), &headPos);

	return headPos;
}

bool Player::IsReloading() const
{
	// TODO
	//return _EntityDataProvider->GetDataFromEntity<edict_t*>(*this, DATA_PLAYER_CURRENTWEAPON);
	return false;
}

bool Player::IsDead() const
{
	return Exists() ? _IIPlayerInfo->IsDead() : true;
}

bool Player::Exists() const
{
	return Entity::Exists() && IsPlayer();
}

bool Player::IsBot() const
{
	return Exists() ? _IIPlayerInfo->IsFakeClient() : false;
}

IPlayerInfo* Player::GetPlayerInfo() const
{
	return _IIPlayerInfo;
}

QAngle Player::GetAngle() const
{
	return Exists() ? _IIPlayerInfo->GetAbsAngles() : QAngle(0.f, 0.f, 0.f);
}

void Player::SetAngle(QAngle angle)
{
	if (Exists())
	{
		// TODO
		//_EntityDataProvider->SetDataOfEntity<QAngle>(*this, DATA_PLAYER_ANGLE, angle);
	}
}