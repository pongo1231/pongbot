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
	DATA_WEAPON_RELOADING
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

private:
	const std::map<EntityDataType, unsigned int> _EntityOffsets =
	{
		{DATA_TEAM, 516},
		{DATA_FLAG_OWNER, 1648},
		{DATA_FLAG_STATUS, 1632},
		{DATA_HEALTH, 244},
		{DATA_PLAYER_FOV, 2876},
		{DATA_PLAYER_CURRENTWEAPON, 2052},
		{DATA_WEAPON_RELOADING, 1706}
	};
};

extern EntityDataProvider *_EntityDataProvider;