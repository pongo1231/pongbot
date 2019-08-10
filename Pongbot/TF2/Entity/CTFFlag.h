#pragma once
#include "Entity.h"
#include "../Objectives/CTFFlagStatusType.h"

class CTFFlag : public Entity
{
public:
	CTFFlag(edict_t* edict) : Entity(edict)
	{}
	CTFFlag(Entity entity) : Entity(entity.GetEdict())
	{}

public:
	int GetOwner() const;
	CTFFlagStatusType GetStatus() const;
	bool IsDisabled() const;
};