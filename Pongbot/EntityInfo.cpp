#include "EntityInfo.h"
#include "EntityDataProvider.h"
#include "Util.h"

edict_t *_MEdict;

edict_t *EntityInfo::_GetEdict() const
{
	return _MEdict;
}

float EntityInfo::GetHealth() const
{
	return _EntityDataProvider->GetDataFromEdict<float>(_GetEdict(), DATA_HEALTH);
}

TFTeam EntityInfo::GetTeam() const
{
	return _EntityDataProvider->GetDataFromEdict<TFTeam>(_GetEdict(), DATA_TEAM);
}

Vector EntityInfo::GetPos() const
{
	return Util::GetEdictOrigin(_GetEdict());
}

bool EntityInfo::IsPlayer() const
{
	return strcmp(_GetEdict()->GetClassName(), "player") == 0;
}