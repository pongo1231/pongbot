#pragma once
#include <hlsdk/public/edict.h>
#include <map>

enum EntityDataType
{
	DATA_TEAM,
	DATA_FLAG_OWNER,
	DATA_FLAG_STATUS,
	DATA_HEALTH
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
	T GetDataFromEdict(edict_t *edict, EntityDataType dataType)
	{
		return (T) *((char*) edict->GetUnknown()->GetBaseEntity() + _EntityOffsets.at(dataType));
	}

private:
	const std::map<EntityDataType, unsigned int> _EntityOffsets =
	{
		{DATA_TEAM, 516},
		{DATA_FLAG_OWNER, 1648},
		{DATA_FLAG_STATUS, 1632},
		{DATA_HEALTH, 244}
	};
};

extern EntityDataProvider *_EntityDataProvider;