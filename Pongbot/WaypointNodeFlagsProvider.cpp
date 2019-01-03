#include "WaypointNodeFlagsProvider.h"
#include "Bot.h"
#include "TFTeam.h"
#include <hlsdk/public/edict.h>

WaypointNodeFlagsProvider *_WaypointNodeFlagsProvider;

std::map<WaypointNodeFlagType, WaypointNodeFlagInfo> _WaypointNodeFlags;

void WaypointNodeFlagsProvider::Init()
{
	Assert(!_WaypointNodeFlagsProvider);
	_WaypointNodeFlagsProvider = new WaypointNodeFlagsProvider();
}

void WaypointNodeFlagsProvider::Destroy()
{
	Assert(_WaypointNodeFlagsProvider);
	delete _WaypointNodeFlagsProvider;
}

std::map<WaypointNodeFlagType, WaypointNodeFlagInfo> WaypointNodeFlagsProvider::GetAllNodeFlags() const
{
	return _WaypointNodeFlags;
}

WaypointNodeFlagInfo WaypointNodeFlagsProvider::GetInfoOfFlagType(WaypointNodeFlagType flagType) const
{
	return _WaypointNodeFlags.at(flagType);
}

int WaypointNodeFlagsProvider::GetInaccessibleNodeFlagsForBot(Bot *bot) const
{
	int inaccessibleNodeFlags = 0;
	inaccessibleNodeFlags |= bot->GetTeam() == TEAM_RED ? NODE_SPAWN_BLUE : NODE_SPAWN_RED;
	return inaccessibleNodeFlags;
}