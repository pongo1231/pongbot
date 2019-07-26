#include "stdafx.h"
#include "WaypointNodeFlagsProvider.h"
#include "Bot.h"
#include "TFTeam.h"
#include "Util.h"

WaypointNodeFlagsProvider* _WaypointNodeFlagsProvider;

void WaypointNodeFlagsProvider::Init()
{
	if (!_WaypointNodeFlagsProvider)
	{
		Util::DebugLog("INIT WaypointNodeFlagsProvider");

		_WaypointNodeFlagsProvider = new WaypointNodeFlagsProvider();
	}
}

void WaypointNodeFlagsProvider::Destroy()
{
	if (_WaypointNodeFlagsProvider)
	{
		Util::DebugLog("DESTROY WaypointNodeFlagsProvider");

		delete _WaypointNodeFlagsProvider;
	}
}

std::map<WaypointNodeFlagType, WaypointNodeFlagInfo> WaypointNodeFlagsProvider::GetAllNodeFlags() const
{
	return _WaypointNodeFlags;
}

WaypointNodeFlagInfo WaypointNodeFlagsProvider::GetInfoOfFlagType(WaypointNodeFlagType flagType) const
{
	return _WaypointNodeFlags.at(flagType);
}

int WaypointNodeFlagsProvider::GetInaccessibleNodeFlagsForBot(Bot* bot) const
{
	unsigned int inaccessibleNodeFlags = 0;
	inaccessibleNodeFlags |= bot->GetTeam() == TEAM_RED ? NODE_SPAWN_BLUE : NODE_SPAWN_RED;
	return inaccessibleNodeFlags;
}