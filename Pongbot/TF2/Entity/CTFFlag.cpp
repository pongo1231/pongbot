#include "stdafx.h"
#include "CTFFlag.h"
#include "EntityDataProvider.h"

int CTFFlag::GetOwner() const
{
	if (!Exists())
	{
		return -1;
	}

	return _EntityDataProvider->GetDataFromEntity<int>(*this, DATA_FLAG_OWNER);
}

CTFFlagStatusType CTFFlag::GetStatus() const
{
	if (!Exists())
	{
		return CTF_UNK;
	}

	return _EntityDataProvider->GetDataFromEntity<CTFFlagStatusType>(*this, DATA_FLAG_STATUS);
}

bool CTFFlag::IsDisabled() const
{
	return _EntityDataProvider->GetDataFromEntity<bool>(*this, DATA_FLAG_DISABLED); 
}