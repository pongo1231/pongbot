//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef CBASE_H
#define CBASE_H
#ifdef _WIN32
#pragma once
#endif

#ifdef _WIN32
// Silence certain warnings
#pragma warning(disable : 4244)		// int or float down-conversion
#pragma warning(disable : 4305)		// int or float data truncation
#pragma warning(disable : 4201)		// nameless struct/union
#pragma warning(disable : 4511)     // copy constructor could not be generated
#pragma warning(disable : 4675)     // resolved overload was found by argument dependent lookup
#endif

#ifdef _DEBUG
#define DEBUG 1
#endif

// Misc C-runtime library headers
#include <math.h>

#include <stdio.h>

// tier 0
#include "hlsdk/public/tier0/dbg.h"
#include "hlsdk/public/tier0/platform.h"
#include "hlsdk/public/tier0/basetypes.h"

// tier 1
#include "hlsdk/public/tier1/strtools.h"
#include "hlsdk/public/tier1/utlvector.h"
#include "hlsdk/public/mathlib/vmatrix.h"

// tier 2
#include "hlsdk/public/string_t.h"

// tier 3
#include "hlsdk/public/vphysics_interface.h"

// Shared engine/DLL constants
#include "hlsdk/public/const.h"
#include "hlsdk/public/edict.h"

// Shared header describing protocol between engine and DLLs
#include "hlsdk/public/eiface.h"
#include "hlsdk/public/iserverentity.h"

#include "hlsdk/public/dt_send.h"

// Shared header between the client DLL and the game DLLs
#include "hlsdk/game/shared/shareddefs.h"
#include "hlsdk/game/shared/ehandle.h"

// app
#if defined(_X360)
#define DISABLE_DEBUG_HISTORY 1
#endif


#include "hlsdk/public/datamap.h"
#include "hlsdk/game/server/util.h"
#include "hlsdk/game/shared/predictable_entity.h"
#include "hlsdk/game/shared/predictableid.h"
#include "hlsdk/game/server/variant_t.h"
#include "hlsdk/game/shared/takedamageinfo.h"
#include "hlsdk/public/tier1/utllinkedlist.h"
#include "hlsdk/game/shared/touchlink.h"
#include "hlsdk/game/shared/groundlink.h"
#include "hlsdk/game/server/base_transmit_proxy.h"
#include "hlsdk/public/soundflags.h"
#include "hlsdk/public/networkvar.h"
#include "hlsdk/game/shared/baseentity_shared.h"
#include "hlsdk/game/server/basetoggle.h"
#include "hlsdk/public/igameevents.h"

// saverestore.h declarations
class ISave;
class IRestore;

// maximum number of targets a single multi_manager entity may be assigned.
#define MAX_MULTI_TARGETS	16 

// NPCEvent.h declarations
struct animevent_t;

struct studiohdr_t;
class CStudioHdr;

extern void FireTargets( const char *targetName, CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

// people gib if their health is <= this at the time of death
#define	GIB_HEALTH_VALUE	-30

#define MAX_OLD_ENEMIES		4 // how many old enemies to remember

// used by suit voice to indicate damage sustained and repaired type to player

enum
{
	itbd_Paralyze = 0,
	itbd_NerveGas,
	itbd_PoisonRecover,
	itbd_Radiation,
	itbd_DrownRecover,
	itbd_Acid,
	itbd_SlowBurn,
	itbd_SlowFreeze,

	// Must be last!
	CDMG_TIMEBASED
};

// when calling KILLED(), a value that governs gib behavior is expected to be 
// one of these three values
#define GIB_NORMAL			0// gib if entity was overkilled
#define GIB_NEVER			1// never gib, no matter how much death damage is done ( freezing, etc )
#define GIB_ALWAYS			2// always gib

class CAI_BaseNPC;
class CAI_ScriptedSequence;
class CSound;

#ifdef _XBOX
//#define FUNCTANK_AUTOUSE  We haven't made the decision to use this yet (sjb)
#else
#undef FUNCTANK_AUTOUSE
#endif//_XBOX

// This is a precompiled header.  Include a bunch of common stuff.
// This is kind of ugly in that it adds a bunch of dependency where it isn't needed.
// But on balance, the compile time is much lower (even incrementally) once the precompiled
// headers contain these headers.
#include "hlsdk/game/shared/precache_register.h"
#include "hlsdk/game/server/baseanimating.h"
#include "hlsdk/game/server/basecombatweapon.h"
#include "hlsdk/game/server/basecombatcharacter.h"
#include "hlsdk/game/shared/gamerules.h"
#include "hlsdk/game/server/entitylist.h"
#include "hlsdk/game/server/basetempentity.h"
#include "hlsdk/game/server/player.h"
#include "hlsdk/game/server/te.h"
#include "hlsdk/game/server/physics.h"
#include "hlsdk/game/server/ndebugoverlay.h"
#include "hlsdk/game/server/recipientfilter.h"

#endif // CBASE_H
