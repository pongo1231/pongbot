#include "CTFFlag.h"
#include "EntityDataProvider.h"

int CTFFlag::GetOwner() const
{
	return _EntityDataProvider->GetDataFromEntity<int>(*this, DATA_FLAG_OWNER);
}

CTFFlagStatusType CTFFlag::GetStatus() const
{
	return _EntityDataProvider->GetDataFromEntity<CTFFlagStatusType>(*this, DATA_FLAG_STATUS);
}