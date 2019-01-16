#pragma once
#include "TFClass.h"
#include <map>

enum TFClassInfoWeaponFlags
{
	WEAPONFLAG_NOATTACK = 1,
	WEAPONFLAG_PRIORITIZE_LONGDIST = 2,
	WEAPONFLAG_PRIORITIZE_MIDDLEDIST = 4,
	WEAPONFLAG_PRIORITIZE_SHORTDIST = 8
};

struct TFClassInfoWeapon
{
	TFClassInfoWeapon(const char *weaponName, unsigned int weaponFlags = 0) : WeaponName(weaponName), WeaponFlags(weaponFlags)
	{}

	const char *WeaponName;
	const unsigned int WeaponFlags;
};

struct TFClassInfo
{
	TFClassInfo(float speed, TFClassInfoWeapon primary, TFClassInfoWeapon secondary,
		TFClassInfoWeapon melee) : Speed(speed), Primary(primary), Secondary(secondary), Melee(melee)
	{}

	const float Speed;
	// TODO: Support for additional weapon types (for example buff banner)
	const TFClassInfoWeapon Primary;
	const TFClassInfoWeapon Secondary;
	const TFClassInfoWeapon Melee;
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
		{CLASS_UNK, TFClassInfo(0.f, TFClassInfoWeapon(""), TFClassInfoWeapon(""), TFClassInfoWeapon(""))},

		{SCOUT, TFClassInfo(133.f,
			TFClassInfoWeapon("TF_WEAPON_SCATTERGUN", WEAPONFLAG_PRIORITIZE_MIDDLEDIST | WEAPONFLAG_PRIORITIZE_SHORTDIST),
			TFClassInfoWeapon("TF_WEAPON_PISTOL_SCOUT", WEAPONFLAG_PRIORITIZE_LONGDIST),
			TFClassInfoWeapon("TF_WEAPON_BAT"))},

		{SOLDIER, TFClassInfo(80.f,
			TFClassInfoWeapon("TF_WEAPON_ROCKETLAUNCHER", WEAPONFLAG_PRIORITIZE_LONGDIST | WEAPONFLAG_PRIORITIZE_MIDDLEDIST),
			TFClassInfoWeapon("TF_WEAPON_SHOTGUN_SOLDIER", WEAPONFLAG_PRIORITIZE_SHORTDIST),
			TFClassInfoWeapon("TF_WEAPON_SHOVEL"))},

		{PYRO, TFClassInfo(100.f,
			TFClassInfoWeapon("TF_WEAPON_FLAMETHROWER", WEAPONFLAG_PRIORITIZE_MIDDLEDIST | WEAPONFLAG_PRIORITIZE_SHORTDIST),
			TFClassInfoWeapon("TF_WEAPON_SHOTGUN_PYRO", WEAPONFLAG_PRIORITIZE_LONGDIST),
			TFClassInfoWeapon("TF_WEAPON_FIREAXE"))},

		{DEMO, TFClassInfo(93.f,
			TFClassInfoWeapon("TF_WEAPON_GRENADELAUNCHER", WEAPONFLAG_PRIORITIZE_LONGDIST | WEAPONFLAG_PRIORITIZE_MIDDLEDIST),
			TFClassInfoWeapon("TF_WEAPON_PIPEBOMBLAUNCHER", WEAPONFLAG_NOATTACK),
			TFClassInfoWeapon("TF_WEAPON_BOTTLE", WEAPONFLAG_PRIORITIZE_SHORTDIST))},

		{HEAVY, TFClassInfo(77.f,
			TFClassInfoWeapon("TF_WEAPON_MINIGUN", WEAPONFLAG_PRIORITIZE_LONGDIST | WEAPONFLAG_PRIORITIZE_MIDDLEDIST | WEAPONFLAG_PRIORITIZE_SHORTDIST),
			TFClassInfoWeapon("TF_WEAPON_SHOTGUN_HWG"),
			TFClassInfoWeapon("TF_WEAPON_FISTS"))},

		{ENGI, TFClassInfo(100.f,
			TFClassInfoWeapon("TF_WEAPON_SHOTGUN_PRIMARY", WEAPONFLAG_PRIORITIZE_MIDDLEDIST | WEAPONFLAG_PRIORITIZE_SHORTDIST),
			TFClassInfoWeapon("TF_WEAPON_PISTOL", WEAPONFLAG_PRIORITIZE_LONGDIST),
			TFClassInfoWeapon("TF_WEAPON_WRENCH"))},

		{MED, TFClassInfo(109.f,
			TFClassInfoWeapon("TF_WEAPON_SYRINGEGUN_MEDIC", WEAPONFLAG_PRIORITIZE_LONGDIST | WEAPONFLAG_PRIORITIZE_MIDDLEDIST),
			TFClassInfoWeapon("TF_WEAPON_MEDIGUN", WEAPONFLAG_NOATTACK),
			TFClassInfoWeapon("TF_WEAPON_BONESAW", WEAPONFLAG_PRIORITIZE_SHORTDIST))},

		{SNIPER, TFClassInfo(100.f,
			TFClassInfoWeapon("TF_WEAPON_SNIPERRIFLE", WEAPONFLAG_PRIORITIZE_LONGDIST),
			TFClassInfoWeapon("TF_WEAPON_SMG", WEAPONFLAG_PRIORITIZE_MIDDLEDIST),
			TFClassInfoWeapon("TF_WEAPON_CLUB", WEAPONFLAG_PRIORITIZE_SHORTDIST))},

		{SPY, TFClassInfo(109.f,
			TFClassInfoWeapon("TF_WEAPON_REVOLVER", WEAPONFLAG_PRIORITIZE_LONGDIST | WEAPONFLAG_PRIORITIZE_MIDDLEDIST),
			TFClassInfoWeapon("TF_WEAPON_SAPPER", WEAPONFLAG_NOATTACK), /* Sapper is using build 3 0 for some stupid reason*/
			TFClassInfoWeapon("TF_WEAPON_KNIFE", WEAPONFLAG_PRIORITIZE_SHORTDIST))}
	};
};

extern TFClassInfoProvider *_TFClassInfoProvider;