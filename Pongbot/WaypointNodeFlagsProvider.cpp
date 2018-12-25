#include "WaypointNodeFlagsProvider.h"
#include <hlsdk/public/edict.h>

WaypointNodeFlagsProvider *_WaypointNodeFlagsProvider;

std::map<WaypointNodeFlagType, WaypointNodeFlagInfo> _WaypointNodeFlags;

WaypointNodeFlagsProvider::WaypointNodeFlagsProvider()
{}

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