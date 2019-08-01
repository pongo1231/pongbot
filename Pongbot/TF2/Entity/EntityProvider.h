#pragma once
#include "../IGameFramable.h"
#include "Entity.h"
#include <vector>

class EntityProvider : public IGameFramable {
private:
	EntityProvider()
	{}

public:
	static void Init();
	static void Destroy();

	std::vector<Entity> GetEntities() const;
	std::vector<Entity> SearchEntitiesByClassname(const char* classname) const;

private:
	std::vector<Entity> _Entities;

	virtual void OnGameFrame();
};

extern EntityProvider* _EntityProvider;