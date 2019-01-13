#include "PlayerInfo.h"
#include "EntityDataProvider.h"
#include <metamod/ISmmAPI.h>

extern IServerGameClients *IIServerGameClients;

float PlayerInfo::GetFOV() const
{
	return _EntityDataProvider->GetDataFromEdict<float>(_GetEdict(), DATA_PLAYER_FOV);
}

bool PlayerInfo::IsSniperZoomedIn() const
{
	return GetFOV() == 20.f;
}

Vector PlayerInfo::GetHeadPos() const
{
	Vector headPos;
	IIServerGameClients->ClientEarPosition(_GetEdict(), &headPos);

	return headPos;
}