//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Client's CBaseCombatWeapon entity
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//
#ifndef C_BASECOMBATWEAPON_H
#define C_BASECOMBATWEAPON_H

#ifdef _WIN32
#pragma once
#endif

#include "hlsdk/game/shared/basecombatweapon_shared.h"
#include "hlsdk/game/client/weapons_resource.h"

class CViewSetup;
class C_BaseViewModel;

// Accessors for local weapons
C_BaseCombatWeapon *GetActiveWeapon( void );


#endif // C_BASECOMBATWEAPON