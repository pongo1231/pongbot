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
	if (!Exists())
		return -1;

	return _EntityDataProvider->GetDataFromEntity<float>(*this, DATA_HEALTH);
}

TFTeam Entity::GetTeam() const
{
	if (!Exists())
		return TEAM_UNK;

	return _EntityDataProvider->GetDataFromEntity<TFTeam>(*this, DATA_TEAM);
}

Vector Entity::GetPos() const
{
	if (!Exists())
		return Vector();

	ICollideable *collideable = GetEdict()->GetCollideable();
	return collideable ? collideable->GetCollisionOrigin() : Vector();
}

bool Entity::IsPlayer() const
{
	return Exists() && strcmp(GetEdict()->GetClassName(), "player") == 0;
}

bool Entity::Exists() const
{
	edict_t *edict = GetEdict();
	return edict && !edict->IsFree();
}