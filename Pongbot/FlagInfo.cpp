#include "FlagInfo.h"
#include "EntityDataProvider.h"

int FlagInfo::GetOwner() const
{
	return _EntityDataProvider->GetDataFromEdict<int>(_GetEdict(), DATA_FLAG_OWNER);
}

CTFFlagStatusType FlagInfo::GetStatus() const
{
	return _EntityDataProvider->GetDataFromEdict<CTFFlagStatusType>(_GetEdict(), DATA_FLAG_STATUS);
}