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
	TFClassInfoWeapon() : _WeaponName("UNK"), _WeaponFlags(0), _Valid(false) {}
	TFClassInfoWeapon(const char* weaponName, unsigned int weaponFlags = 0) : _WeaponName(weaponName), _WeaponFlags(weaponFlags), _Valid(true) {}
	int operator=(const TFClassInfoWeapon& target)
	{
		_WeaponName = target.GetWeaponName();
		_WeaponFlags = target.GetWeaponFlags();
		_Valid = target.IsValid();
	}

	const char* GetWeaponName() const
	{
		return _WeaponName;
	}

	unsigned int GetWeaponFlags() const
	{
		return _WeaponFlags;
	}

	bool IsValid() const
	{
		return _Valid;
	}

private:
	const char* _WeaponName;
	unsigned int _WeaponFlags;
	bool _Valid;
};

struct TFClassInfo
{
	TFClassInfo() : _Speed(0.f), _Valid(false) {}
	TFClassInfo(float speed, TFClassInfoWeapon primary, TFClassInfoWeapon secondary,
		TFClassInfoWeapon melee) : _Speed(speed), _Primary(primary), _Secondary(secondary), _Melee(melee), _Valid(true) {}
	int operator=(const TFClassInfo& target)
	{
		_Speed = target.GetSpeed();
		_Primary = target.GetPrimary();
		_Secondary = target.GetSecondary();
		_Melee = target.GetMelee();
		_Valid = target.IsValid();
	}

	float GetSpeed() const
	{
		return _Speed;
	}

	TFClassInfoWeapon GetPrimary() const
	{
		return _Primary;
	}

	TFClassInfoWeapon GetSecondary() const
	{
		return _Secondary;
	}

	TFClassInfoWeapon GetMelee() const
	{
		return _Melee;
	}

	bool IsValid() const
	{
		return _Valid && _Primary.IsValid() && _Secondary.IsValid() && _Melee.IsValid();
	}

private:
	float _Speed;
	// TODO: Support for additional weapon types (for example buff banner)
	TFClassInfoWeapon _Primary;
	TFClassInfoWeapon _Secondary;
	TFClassInfoWeapon _Melee;
	bool _Valid;
};

class TFClassInfoProvider
{
private:
	TFClassInfoProvider() {}

public:
	static void Init();
	static void Destroy();

	TFClassInfo GetClassInfo(TFClass tfClass) const;

private:
	const std::map<TFClass, TFClassInfo> _ClassInfos =
	{
		{CLASS_UNK, {}},

		{SCOUT, {133.f,
			TFClassInfoWeapon("TF_WEAPON_SCATTERGUN", WEAPONFLAG_PRIORITIZE_MIDDLEDIST | WEAPONFLAG_PRIORITIZE_SHORTDIST),
			TFClassInfoWeapon("TF_WEAPON_PISTOL_SCOUT", WEAPONFLAG_PRIORITIZE_LONGDIST),
			TFClassInfoWeapon("TF_WEAPON_BAT")}},

		{SOLDIER, {80.f,
			TFClassInfoWeapon("TF_WEAPON_ROCKETLAUNCHER", WEAPONFLAG_PRIORITIZE_LONGDIST | WEAPONFLAG_PRIORITIZE_MIDDLEDIST),
			TFClassInfoWeapon("TF_WEAPON_SHOTGUN_SOLDIER", WEAPONFLAG_PRIORITIZE_SHORTDIST),
			TFClassInfoWeapon("TF_WEAPON_SHOVEL")}},

		{PYRO, {100.f,
			TFClassInfoWeapon("TF_WEAPON_FLAMETHROWER", WEAPONFLAG_PRIORITIZE_MIDDLEDIST | WEAPONFLAG_PRIORITIZE_SHORTDIST),
			TFClassInfoWeapon("TF_WEAPON_SHOTGUN_PYRO", WEAPONFLAG_PRIORITIZE_LONGDIST),
			TFClassInfoWeapon("TF_WEAPON_FIREAXE")}},

		{DEMO, {93.f,
			TFClassInfoWeapon("TF_WEAPON_GRENADELAUNCHER", WEAPONFLAG_PRIORITIZE_LONGDIST | WEAPONFLAG_PRIORITIZE_MIDDLEDIST),
			TFClassInfoWeapon("TF_WEAPON_PIPEBOMBLAUNCHER", WEAPONFLAG_NOATTACK),
			TFClassInfoWeapon("TF_WEAPON_BOTTLE", WEAPONFLAG_PRIORITIZE_SHORTDIST)}},

		{HEAVY, {77.f,
			TFClassInfoWeapon("TF_WEAPON_MINIGUN", WEAPONFLAG_PRIORITIZE_LONGDIST | WEAPONFLAG_PRIORITIZE_MIDDLEDIST | WEAPONFLAG_PRIORITIZE_SHORTDIST),
			TFClassInfoWeapon("TF_WEAPON_SHOTGUN_HWG"),
			TFClassInfoWeapon("TF_WEAPON_FISTS")}},

		{ENGI, {100.f,
			TFClassInfoWeapon("TF_WEAPON_SHOTGUN_PRIMARY", WEAPONFLAG_PRIORITIZE_MIDDLEDIST | WEAPONFLAG_PRIORITIZE_SHORTDIST),
			TFClassInfoWeapon("TF_WEAPON_PISTOL", WEAPONFLAG_PRIORITIZE_LONGDIST),
			TFClassInfoWeapon("TF_WEAPON_WRENCH")}},

		{MED, {109.f,
			TFClassInfoWeapon("TF_WEAPON_SYRINGEGUN_MEDIC", WEAPONFLAG_PRIORITIZE_LONGDIST | WEAPONFLAG_PRIORITIZE_MIDDLEDIST),
			TFClassInfoWeapon("TF_WEAPON_MEDIGUN", WEAPONFLAG_NOATTACK),
			TFClassInfoWeapon("TF_WEAPON_BONESAW", WEAPONFLAG_PRIORITIZE_SHORTDIST)}},

		{SNIPER, {100.f,
			TFClassInfoWeapon("TF_WEAPON_SNIPERRIFLE", WEAPONFLAG_PRIORITIZE_LONGDIST),
			TFClassInfoWeapon("TF_WEAPON_SMG", WEAPONFLAG_PRIORITIZE_MIDDLEDIST),
			TFClassInfoWeapon("TF_WEAPON_CLUB", WEAPONFLAG_PRIORITIZE_SHORTDIST)}},

		{SPY, {109.f,
			TFClassInfoWeapon("TF_WEAPON_REVOLVER", WEAPONFLAG_PRIORITIZE_LONGDIST | WEAPONFLAG_PRIORITIZE_MIDDLEDIST),
			TFClassInfoWeapon("TF_WEAPON_SAPPER", WEAPONFLAG_NOATTACK), /* Sapper is using build 3 0 for some stupid reason*/
			TFClassInfoWeapon("TF_WEAPON_KNIFE", WEAPONFLAG_PRIORITIZE_SHORTDIST)}}
	};
};

extern TFClassInfoProvider *_TFClassInfoProvider;