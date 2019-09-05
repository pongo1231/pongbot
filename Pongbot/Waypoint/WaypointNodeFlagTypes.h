#pragma once

enum WaypointNodeFlagType
{
	NODE_SPAWN_RED = (1 << 0),
	NODE_SPAWN_BLUE = (1 << 1),
	NODE_ITEMFLAG_RED = (1 << 2),
	NODE_ITEMFLAG_BLUE = (1 << 3),
	NODE_HEALTH = (1 << 4),
	NODE_AMMO = (1 << 5),
	NODE_SENTRY_RED = (1 << 6),
	NODE_SENTRY_BLUE = (1 << 7),
	NODE_SNIPER_CAMP_RED = (1 << 8),
	NODE_SNIPER_CAMP_BLUE = (1 << 9)
};