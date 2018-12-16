#pragma once
#include "GameFramable.h"
#include <hlsdk/public/edict.h>
#include <vector>

class EdictsProvider : public GameFramable {
private:
	EdictsProvider();

public:
	static void Init();
	static void Destroy();

	std::vector<edict_t*> GetEdicts() const;
	std::vector<edict_t*> SearchEdictsByClassname(const char *classname) const;

private:
	std::vector<edict_t*> _Edicts;

	void OnGameFrame();
};

extern EdictsProvider *_EdictsProvider;