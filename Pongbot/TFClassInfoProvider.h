#pragma once
#include "TFClass.h"
#include <map>

struct TFClassInfo
{
	TFClassInfo(float speed, const char *primary, const char *secondary,
		const char *melee) : Speed(speed), Primary(primary), Secondary(secondary), Melee(melee)
	{}

	const float Speed;
	// TODO: Support for additional weapon types (for example buff banner)
	const char *Primary;
	const char *Secondary;
	const char *Melee;
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
		{CLASS_UNKNOWN, TFClassInfo(0.f, "", "", "")},
		{SCOUT, TFClassInfo(133.f, "TF_WEAPON_PISTOL_SCOUT", "TF_WEAPON_SCATTERGUN", "TF_WEAPON_BAT")},
		{SOLDIER, TFClassInfo(80.f, "TF_WEAPON_ROCKETLAUNCHER", "TF_WEAPON_SHOTGUN_SOLDIER", "TF_WEAPON_SHOVEL")},
		{PYRO, TFClassInfo(100.f, "TF_WEAPON_SHOTGUN_PYRO", "TF_WEAPON_FLAMETHROWER", "TF_WEAPON_FIREAXE")},
		{DEMO, TFClassInfo(93.f, "TF_WEAPON_GRENADELAUNCHER", "TF_WEAPON_PIPEBOMBLAUNCHER", "TF_WEAPON_BOTTLE")},
		{HEAVY, TFClassInfo(77.f, "TF_WEAPON_MINIGUN", "TF_WEAPON_HWG", "TF_WEAPON_FISTS")},
		{ENGI, TFClassInfo(100.f, "TF_WEAPON_PISTOL", "TF_WEAPON_SHOTGUN_PRIMARY", "TF_WEAPON_WRENCH")},
		{MED, TFClassInfo(109.f, "TF_WEAPON_SYRINGEGUN_MEDIC", "TF_WEAPON_MEDIGUN", "TF_WEAPON_BONESAW")},
		{SNIPER, TFClassInfo(100.f, "TF_WEAPON_SNIPERRIFLE", "TF_WEAPON_SMG", "TF_WEAPON_CLUB")},
		{SPY, TFClassInfo(109.f, "TF_WEAPON_REVOLVER", "TF_WEAPON_SAPPER" /* Sapper is using build 3 0 for some stupid reason*/, "TF_WEAPON_KNIFE")}
	};
};

extern TFClassInfoProvider *_TFClassInfoProvider;