#include "WaypointNodeFlagsProvider.h"
#include "Bot.h"
#include "TFTeam.h"

WaypointNodeFlagsProvider *_WaypointNodeFlagsProvider;

std::map<WaypointNodeFlagType, WaypointNodeFlagInfo> _WaypointNodeFlags;

void WaypointNodeFlagsProvider::Init()
{
	if (!_WaypointNodeFlagsProvider)
		_WaypointNodeFlagsProvider = new WaypointNodeFlagsProvider();
}

void WaypointNodeFlagsProvider::Destroy()
{
	if (_WaypointNodeFlagsProvider)
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
	unsigned int inaccessibleNodeFlags = 0;
	inaccessibleNodeFlags |= bot->GetTeam() == TEAM_RED ? NODE_SPAWN_BLUE : NODE_SPAWN_RED;
	return inaccessibleNodeFlags;
}