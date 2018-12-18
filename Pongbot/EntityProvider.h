#pragma once
#include "IGameFramable.h"
#include <hlsdk/public/edict.h>
#include <vector>

class EntityProvider : public IGameFramable {
private:
	EntityProvider();

public:
	static void Init();
	static void Destroy();

	std::vector<edict_t*> GetEdicts() const;
	std::vector<edict_t*> SearchEdictsByClassname(const char *classname) const;

private:
	std::vector<edict_t*> _Edicts;

	void OnGameFrame();
};

extern EntityProvider *_EntityProvider;