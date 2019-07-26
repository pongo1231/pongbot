#pragma once
#include "IGameFramable.h"
#include "Entity.h"
#include <vector>

class BotVisiblesProvider : public IGameFramable
{
private:
	BotVisiblesProvider()
	{}

public:
	static void Init();
	static void Destroy();

	std::vector<Entity> GetVisibleEntities() const;
	virtual void OnGameFrame();

private:
	std::vector<Entity> _VisibleEntities;

	bool _IsEntityRelevant(Entity entity) const;
};

extern BotVisiblesProvider* _BotVisiblesProvider;