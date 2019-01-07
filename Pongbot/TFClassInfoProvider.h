#pragma once
#include "TFClass.h"
#include <map>

struct TFClassInfo
{
	TFClassInfo(float speed, bool swapPrimaryWithSecondary) : Speed(speed), PrimaryWeaponSwap(swapPrimaryWithSecondary)
	{}

	const float Speed;
	const bool PrimaryWeaponSwap;
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
		{SCOUT, TFClassInfo(133.f, false)},
		{SOLDIER, TFClassInfo(80.f, true)},
		{PYRO, TFClassInfo(100.f, true)},
		{DEMO, TFClassInfo(93.f, false)},
		{HEAVY, TFClassInfo(77.f, false)},
		{ENGI, TFClassInfo(100.f, false)},
		{MED, TFClassInfo(109.f, false)},
		{SNIPER, TFClassInfo(100.f, true)},
		{SPY, TFClassInfo(109.f, false)}
	};
};

extern TFClassInfoProvider *_TFClassInfoProvider;