#include "Entity.h"
#include "EntityDataProvider.h"
#include "Util.h"

edict_t *_MEdict;

edict_t *Entity::GetEdict() const
{
	return _MEdict;
}

float Entity::GetHealth() const
{
	return _EntityDataProvider->GetDataFromEdict<float>(GetEdict(), DATA_HEALTH);
}

TFTeam Entity::GetTeam() const
{
	return _EntityDataProvider->GetDataFromEdict<TFTeam>(GetEdict(), DATA_TEAM);
}

Vector Entity::GetPos() const
{
	return Util::GetEdictOrigin(GetEdict());
}

bool Entity::IsPlayer() const
{
	return strcmp(GetEdict()->GetClassName(), "player") == 0;
}

bool Entity::Exists() const
{
	return GetEdict() != nullptr;
}