#pragma once
#include "WaypointNodeFlagTypes.h"
#include <map>
#include <vector>

class Bot;

struct WaypointNodeFlagInfo
{
	WaypointNodeFlagInfo(const char *name, const char *desc) : Name(name), Desc(desc)
	{}

	const char *Name;
	const char *Desc;
};

class WaypointNodeFlagsProvider
{
private:
	WaypointNodeFlagsProvider()
	{}

public:
	static void Init();
	static void Destroy();

	std::map<WaypointNodeFlagType, WaypointNodeFlagInfo> GetAllNodeFlags() const;
	WaypointNodeFlagInfo GetInfoOfFlagType(WaypointNodeFlagType flagType) const;
	int GetInaccessibleNodeFlagsForBot(Bot *bot) const;

private:
	std::map<WaypointNodeFlagType, WaypointNodeFlagInfo> _WaypointNodeFlags =
	{
		{SPAWN_RED, WaypointNodeFlagInfo("SPAWN_RED", "Inside red spawn zone")},
		{SPAWN_BLUE, WaypointNodeFlagInfo("SPAWN_BLUE", "Inside blue spawn zone")},
		{ITEMFLAG_RED, WaypointNodeFlagInfo("ITEMFLAG_RED", "Red CTF Flag spawns here")},
		{ITEMFLAG_BLUE, WaypointNodeFlagInfo("ITEMFLAG_BLUE", "Blue CTF Flag spawns here")},
		{HEALTH, WaypointNodeFlagInfo("HEALTH", "Health Pack spawns here")},
		{AMMO, WaypointNodeFlagInfo("AMMO", "Ammo Pack spawns here")},
	};
};

extern WaypointNodeFlagsProvider *_WaypointNodeFlagsProvider;