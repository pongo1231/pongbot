#pragma once
#include "TFClass.h"
#include <map>

struct TFClassInfo
{
	TFClassInfo(float speed) : Speed(speed)
	{}

	const float Speed;
};

class TFClassInfoProvider
{
private:
	TFClassInfoProvider()
	{}

public:
	static void Init();
	static void Destroy();

	TFClassInfo GetClassInfo(TFClass tfClass) const;

private:
	const std::map<TFClass, TFClassInfo> _ClassInfos =
	{
		{SCOUT, TFClassInfo(133.f)},
		{SOLDIER, TFClassInfo(80.f)},
		{PYRO, TFClassInfo(100.f)},
		{DEMO, TFClassInfo(93.f)},
		{HEAVY, TFClassInfo(77.f)},
		{ENGI, TFClassInfo(100.f)},
		{MED, TFClassInfo(109.f)},
		{SNIPER, TFClassInfo(100.f)},
		{SPY, TFClassInfo(109.f)}
	};
};

extern TFClassInfoProvider *_TFClassInfoProvider;