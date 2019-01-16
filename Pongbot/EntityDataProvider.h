#pragma once
#include "Util.h"
#include "Entity.h"
#include <hlsdk/public/edict.h>
#include <map>

enum EntityDataType
{
	DATA_TEAM,
	DATA_FLAG_OWNER,
	DATA_FLAG_STATUS,
	DATA_HEALTH,
	DATA_PLAYER_FOV,
	DATA_PLAYER_CURRENTWEAPON,
	DATA_WEAPON_RELOADING,
	DATA_PLAYER_HEADSCALE
};

class EntityDataProvider
{
private:
	EntityDataProvider()
	{}

public:
	static void Init();
	static void Destroy();

	template<typename T>
	T GetDataFromEntity(Entity entity, EntityDataType dataType)
	{
		if (!entity.Exists() || !entity.GetEdict()->GetUnknown())
			return (T) 0;

		return (T) *((char*) entity.GetEdict()->GetUnknown()->GetBaseEntity() + _EntityOffsets.at(dataType));
	}

	template<typename T>
	void SetDataFromEntity(Entity entity, EntityDataType dataType, T data)
	{
		if (entity.Exists() && entity.GetEdict()->GetUnknown())
			*((char*) entity.GetEdict()->GetUnknown()->GetBaseEntity() + _EntityOffsets.at(dataType)) = data;
	}

private:
	const std::map<EntityDataType, unsigned int> _EntityOffsets =
	{
		{DATA_TEAM, 516},
		{DATA_FLAG_OWNER, 1648},
		{DATA_FLAG_STATUS, 1632},
		{DATA_HEALTH, 244},
		{DATA_PLAYER_FOV, 2876},
		{DATA_PLAYER_CURRENTWEAPON, 2052}
	};
};

extern EntityDataProvider *_EntityDataProvider;