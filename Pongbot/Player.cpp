#include "Player.h"
#include "EntityDataProvider.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/game/server/iplayerinfo.h>

extern IServerGameClients *IIServerGameClients;
extern IPlayerInfoManager *IIPlayerInfoManager;

IPlayerInfo *_IIPlayerInfo;

Player::Player(edict_t *edict) : Entity(edict), _IIPlayerInfo(IIPlayerInfoManager->GetPlayerInfo(edict))
{}

Player::Player(Entity entity) : Entity(entity.GetEdict()), _IIPlayerInfo(IIPlayerInfoManager->GetPlayerInfo(entity.GetEdict()))
{}

float Player::GetFOV() const
{
	return _EntityDataProvider->GetDataFromEntity<float>(*this, DATA_PLAYER_FOV);
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
	//return _EntityDataProvider->GetDataFromEntity<edict_t*>(*this, DATA_PLAYER_CURRENTWEAPON);
	return false;
}

bool Player::IsConnected() const
{
	return _IIPlayerInfo->IsConnected();
}