#pragma once
#include "WaypointNodeFlagTypes.h"
#include <map>
#include <vector>

class Bot;

struct WaypointNodeFlagInfo
{
	WaypointNodeFlagInfo(const char* name, const char* desc) : Name(name), Desc(desc) {}

	const char* Name;
	const char* Desc;
};

class WaypointNodeFlagsProvider
{
private:
	WaypointNodeFlagsProvider() {}

public:
	static void Init();
	static void Destroy();

	std::map<WaypointNodeFlagType, WaypointNodeFlagInfo> GetAllNodeFlags() const;
	WaypointNodeFlagInfo GetInfoOfFlagType(WaypointNodeFlagType flagType) const;
	int GetInaccessibleNodeFlagsForBot(Bot* bot) const;

private:
	std::map<WaypointNodeFlagType, WaypointNodeFlagInfo> _WaypointNodeFlags =
	{
		{NODE_SPAWN_RED, {"SPAWN_RED", "Inside red spawn zone"}},
		{NODE_SPAWN_BLUE, {"SPAWN_BLUE", "Inside blue spawn zone"}},
		{NODE_ITEMFLAG_RED, {"ITEMFLAG_RED", "Red CTF Flag spawns here"}},
		{NODE_ITEMFLAG_BLUE, {"ITEMFLAG_BLUE", "Blue CTF Flag spawns here"}},
		{NODE_HEALTH, {"HEALTH", "Health Pack spawns here"}},
		{NODE_AMMO, {"AMMO", "Ammo Pack spawns here"}},
		{NODE_SENTRY_RED, {"SENTRY_RED", "Red Engineers build sentries here"}},
		{NODE_SENTRY_BLUE, {"SENTRY_BLUE", "Blue Engineers build sentries here"}},
		{NODE_SNIPER_CAMP_RED, {"SNIPER_CAMP_RED", "Red Snipers camp here"}},
		{NODE_SNIPER_CAMP_BLUE, {"SNIPER_CAMP_BLUE", "Blue Snipers camp here"}}
	};
};

extern WaypointNodeFlagsProvider* _WaypointNodeFlagsProvider;