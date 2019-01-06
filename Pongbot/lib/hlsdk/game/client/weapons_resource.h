//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef WEAPONS_RESOURCE_H
#define WEAPONS_RESOURCE_H
#pragma once

#include "hlsdk/game/shared/shareddefs.h"
#include "hlsdk/game/shared/weapon_parse.h"
#include "hlsdk/public/tier1/UtlDict.h"
#include "hlsdk/game/client/hud.h"

class C_BaseCombatWeapon;
class CHudTexture;

//-----------------------------------------------------------------------------
// Purpose: Stores data about the Weapon Definitions passed to the client when
//			the client first connects to a server. 
//-----------------------------------------------------------------------------
class WeaponsResource
{
public:
	WeaponsResource( void );
	~WeaponsResource( void );

	void Init( void );
	void Reset( void );

	// Sprite handling
	void LoadWeaponSprites( WEAPON_FILE_INFO_HANDLE hWeaponFileInfo );
	void LoadAllWeaponSprites( void );

	// Ammo Handling
	CHudTexture					*GetAmmoIconFromWeapon( int iAmmoId );
	const FileWeaponInfo_t		*GetWeaponFromAmmo( int iAmmoId );
};

extern WeaponsResource gWR;

#endif // WEAPONS_RESOURCE_H
