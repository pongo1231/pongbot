#pragma once
#include "../../IGameFramable.h"
#include "Entity.h"
#include <vector>

class EntityProvider : public IGameFramable
{
private:
	EntityProvider() {}

public:
	static void Init();
	static void Destroy();

	std::vector<Entity> GetEntities() const;
	std::vector<Entity> SearchEntitiesByClassname(const char* className);

private:
	std::vector<Entity> _Entities;

	void _UpdateEntities();
	virtual void OnGameFrame();
};

extern EntityProvider* _EntityProvider;