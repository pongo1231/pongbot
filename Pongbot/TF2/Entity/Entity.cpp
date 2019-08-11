#include <stdafx.h>
#include "Entity.h"
#include "EntityDataProvider.h"
#include "../../Util.h"
#include <hlsdk/public/edict.h>
#include <hlsdk/public/mathlib/vector.h>

edict_t* Entity::GetEdict() const
{
	return _MEdict;
}

float Entity::GetHealth() const
{
	if (!Exists())
	{
		return -1;
	}

	return _EntityDataProvider->GetDataFromEntity<float>(*this, DATA_HEALTH);
}

TFTeam Entity::GetTeam() const
{
	if (!Exists())
	{
		return TEAM_UNK;
	}

	return _EntityDataProvider->GetDataFromEntity<TFTeam>(*this, DATA_TEAM);
}

Vector Entity::GetPos() const
{
	if (!Exists())
	{
		return Vector(0.f, 0.f, 0.f);
	}

	ICollideable* collideable = GetEdict()->GetCollideable();
	return collideable ? collideable->GetCollisionOrigin() : Vector(0.f, 0.f, 0.f);
}

bool Entity::IsPlayer() const
{
	return Exists() && strcmp(GetEdictClassName(), "player") == 0;
}

bool Entity::Exists() const
{
	return GetEdict();
}

const char* Entity::GetEdictClassName() const
{
	if (!Exists())
	{
		return nullptr;
	}

	return GetEdict()->GetClassName();
}