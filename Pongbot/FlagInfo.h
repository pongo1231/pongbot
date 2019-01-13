#pragma once
#include "EntityInfo.h"
#include "CTFFlagStatusType.h"

class FlagInfo : public EntityInfo
{
public:
	FlagInfo(edict_t *edict) : EntityInfo(edict)
	{}

public:
	int GetOwner() const;
	CTFFlagStatusType GetStatus() const;
};