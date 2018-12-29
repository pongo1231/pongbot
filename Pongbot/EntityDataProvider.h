#pragma once
#include <hlsdk/public/edict.h>
#include <map>

enum EntityDataType
{
	TEAM,
	FLAG_OWNER,
	FLAG_STATUS
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
		{TEAM, 516},
		{FLAG_OWNER, 1648},
		{FLAG_STATUS, 1632}
	};
};

extern EntityDataProvider *_EntityDataProvider;