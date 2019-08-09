#pragma once
#include "../../Util.h"
#include "Entity.h"
#include <hlsdk/public/edict.h>
#include <map>

enum EntityDataType
{
	DATA_TEAM, // CBaseEntity::m_iTeamNum
	DATA_FLAG_OWNER, // CCaptureFlag::m_hPrevOwner
	DATA_FLAG_STATUS, // CCaptureFlag::m_nFlagStatus
	DATA_HEALTH, // CBaseObject::m_iHealth
	DATA_PLAYER_FOV, // CBasePlayer::m_iFOV
	DATA_PLAYER_CURRENTWEAPON // CBaseCombatCharacter::m_hActiveWeapon
};

class EntityDataProvider
{
private:
	EntityDataProvider() {}

public:
	static void Init();
	static void Destroy();

	template<typename T>
	T GetDataFromEntity(Entity entity, EntityDataType dataType)
	{
		if (!entity.Exists() || !entity.GetEdict()->GetUnknown())
		{
			return (T) 0;
		}

		return (T) *((char*) entity.GetEdict()->GetUnknown()->GetBaseEntity() + _EntityOffsets.at(dataType));
	}

	template<typename T>
	void SetDataFromEntity(Entity entity, EntityDataType dataType, T data)
	{
		if (entity.Exists() && entity.GetEdict()->GetUnknown())
		{
			*((char*)entity.GetEdict()->GetUnknown()->GetBaseEntity() + _EntityOffsets.at(dataType)) = data;
		}
	}

private:
	const std::map<EntityDataType, unsigned int> _EntityOffsets =
	{
		#ifdef _WIN32
		{DATA_TEAM, 516},
		{DATA_FLAG_OWNER, 1648},
		{DATA_FLAG_STATUS, 1632},
		{DATA_HEALTH, 244},
		{DATA_PLAYER_FOV, 2876},
		{DATA_PLAYER_CURRENTWEAPON, 2052}
		#elif _LINUX
		{DATA_TEAM, 536},
		{DATA_FLAG_OWNER, 1668},
		{DATA_FLAG_STATUS, 1652},
		{DATA_HEALTH, 264},
		{DATA_PLAYER_FOV, 2896},
		{DATA_PLAYER_CURRENTWEAPON, 2072}
		#endif
	};
};

extern EntityDataProvider* _EntityDataProvider;