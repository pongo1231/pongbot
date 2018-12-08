#include "BotVisiblesProvider.h"

extern IVEngineServer *Engine;

BotVisiblesProvider *_BotVisiblesProvider;

std::vector<edict_t*> _Edicts;

BotVisiblesProvider::BotVisiblesProvider() {}

void BotVisiblesProvider::Init() {
	Assert(!_BotVisiblesProvider);
	_BotVisiblesProvider = new BotVisiblesProvider();
}

void BotVisiblesProvider::Destroy() {
	Assert(_BotVisiblesProvider);
	delete _BotVisiblesProvider;
}

std::vector<edict_t*> BotVisiblesProvider::GetAllEdicts() const {
	return _Edicts;
}

void BotVisiblesProvider::OnGameFrame() {
	static float waitTime;
	float currentTime = Engine->Time();
	if (waitTime > currentTime)
		return;
	waitTime = currentTime + 5;

	_Edicts.clear();

	for (int i = 1; i < Engine->GetEntityCount(); i++) {
		edict_t *edict = Engine->PEntityOfEntIndex(i);
		if (edict)
			_Edicts.push_back(edict);
	}
}