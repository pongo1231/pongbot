#include "Entity.h"
#include "EntityDataProvider.h"
#include "Util.h"
#include <hlsdk/public/edict.h>
#include <hlsdk/public/mathlib/vector.h>

edict_t *_MEdict;

edict_t *Entity::GetEdict() const
{
	return _MEdict;
}

float Entity::GetHealth() const
{
	return _EntityDataProvider->GetDataFromEntity<float>(*this, DATA_HEALTH);
}

TFTeam Entity::GetTeam() const
{
	return _EntityDataProvider->GetDataFromEntity<TFTeam>(*this, DATA_TEAM);
}

Vector Entity::GetPos() const
{
	ICollideable *collideable = GetEdict()->GetCollideable();
	return collideable ? collideable->GetCollisionOrigin() : Vector();
}

bool Entity::IsPlayer() const
{
	return strcmp(GetEdict()->GetClassName(), "player") == 0;
}

bool Entity::Exists() const
{
	return GetEdict();
}

bool Entity::IsDead() const
{
	return GetHealth() <= 0.f;
}