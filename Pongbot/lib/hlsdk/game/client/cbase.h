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

struct studiohdr_t;

#include <stdio.h>
#include <stdlib.h>

#include <hlsdk/public/tier0/platform.h>
#include <hlsdk/public/tier0/dbg.h>

#include <hlsdk/public/tier1/strtools.h>
#include <hlsdk/public/vstdlib/random.h>
#include <hlsdk/public/tier1/utlvector.h>

#include <hlsdk/public/const.h>

#include "hlsdk/public/string_t.h"

// These two have to be included very early
#include <hlsdk/game/shared/predictableid.h>
#include <hlsdk/game/shared/predictable_entity.h>

#include "cdll_util.h"
#include <hlsdk/game/shared/util_shared.h>

#include <hlsdk/public/icvar.h>
#include <hlsdk/game/shared/baseentity_shared.h>


// This is a precompiled header.  Include a bunch of common stuff.
// This is kind of ugly in that it adds a bunch of dependency where it isn't needed.
// But on balance, the compile time is much lower (even incrementally) once the precompiled
// headers contain these headers.
#include "hlsdk/game/shared/precache_register.h"
#include "hlsdk/game/client/c_basecombatweapon.h"
#include "hlsdk/game/client/c_basecombatcharacter.h"
#include "hlsdk/game/shared/gamerules.h"
#include "hlsdk/game/client/c_baseplayer.h"
#include "hlsdk/game/shared/itempents.h"
#include "hlsdk/public/vphysics_interface.h"
#include "hlsdk/game/server/physics.h"
#include "hlsdk/game/client/c_recipientfilter.h"
#include "hlsdk/game/client/cdll_client_int.h"
#include "hlsdk/public/worldsize.h"
#include "hlsdk/public/engine/ivmodelinfo.h"

#endif // CBASE_H
