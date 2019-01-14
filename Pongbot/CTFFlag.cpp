#include "CTFFlag.h"
#include "EntityDataProvider.h"

int CTFFlag::GetOwner() const
{
	return _EntityDataProvider->GetDataFromEdict<int>(GetEdict(), DATA_FLAG_OWNER);
}

CTFFlagStatusType CTFFlag::GetStatus() const
{
	return _EntityDataProvider->GetDataFromEdict<CTFFlagStatusType>(GetEdict(), DATA_FLAG_STATUS);
}