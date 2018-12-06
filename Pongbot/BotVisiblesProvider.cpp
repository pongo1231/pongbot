#include "BotVisiblesProvider.h"

extern IVEngineServer *Engine;

BotVisiblesProvider *_BotVisiblesProvider;

std::vector<CBaseEntity*> _Entities;

BotVisiblesProvider::BotVisiblesProvider() {}

void BotVisiblesProvider::Init() {
	Assert(!_BotVisiblesProvider);
	_BotVisiblesProvider = new BotVisiblesProvider();
}

void BotVisiblesProvider::Destroy() {
	Assert(_BotVisiblesProvider);
	delete _BotVisiblesProvider;
}

std::vector<CBaseEntity*> BotVisiblesProvider::GetEntities() {
	return _Entities;
}

void BotVisiblesProvider::OnGameFrame() {
	static float waitTime;
	float currentTime = Engine->Time();
	if (waitTime > currentTime)
		return;
	waitTime = currentTime + 10;

	_Entities.clear();

	for (unsigned int i = 1; i < (unsigned int) Engine->GetEntityCount(); i++) {
		edict_t *edict = Engine->PEntityOfEntIndex(i);
		if (edict) {
			IServerEntity *serverEntity = edict->GetIServerEntity();
			if (serverEntity) {
				CBaseEntity *entity = serverEntity->GetBaseEntity();
				entity->
				if (entity)
					_Entities.push_back(entity);
			}
		}
	}
}