#include "EdictsProvider.h"
#include <metamod/ISmmAPI.h>

#define EDICT_PROVIDER_TICK 1

extern IVEngineServer *Engine;

EdictsProvider *_EdictsProvider;

std::vector<edict_t*> _Edicts;

EdictsProvider::EdictsProvider() {}

void EdictsProvider::Init() {
	Assert(!_EdictsProvider);
	_EdictsProvider = new EdictsProvider();
}

void EdictsProvider::Destroy() {
	Assert(_EdictsProvider);
	delete _EdictsProvider;
}

std::vector<edict_t*> EdictsProvider::GetEdicts() const {
	return _Edicts;
}

std::vector<edict_t*> EdictsProvider::SearchEdictsByClassname(const char *classname) const {
	std::vector<edict_t*> foundEdicts;
	for (edict_t *edict : _Edicts)
		if (strcmp(edict->GetClassName(), classname) == 0)
			foundEdicts.push_back(edict);
	return foundEdicts;
}

void EdictsProvider::OnGameFrame() {
	static float tickTime;
	float currentTime = Engine->Time();
	if (tickTime > currentTime)
		return;
	tickTime = currentTime + EDICT_PROVIDER_TICK;

	_Edicts.clear();
	for (int i = 1; i < Engine->GetEntityCount(); i++) {
		edict_t *edict = Engine->PEntityOfEntIndex(i);
		if (edict)
			_Edicts.push_back(edict);
	}
}