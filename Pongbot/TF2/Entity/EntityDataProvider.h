#pragma once
#include "../../Util.h"
#include "Entity.h"
#include <hlsdk/public/edict.h>
#include <map>

enum EntityDataType
{
	DATA_TEAM, // CBaseEntity::m_iTeamNum
	DATA_FLAG_OWNER, // CCaptureFlag::m_hPrevOwner
	DATA_FLAG_STATUS, // CCaptureFlag::m_nFlagStatus
	DATA_FLAG_DISABLED, // CCaptureFlag::m_bDisabled
	DATA_HEALTH, // CBaseObject::m_iHealth
	DATA_PLAYER_FOV, // CBasePlayer::m_iFOV
	DATA_PLAYER_CURRENTWEAPON, // CBaseCombatCharacter::m_hActiveWeapon
	DATA_PLAYER_ANGLE, // CBaseEntity::m_angRotation
	DATA_ROUND_TIMER_STATE, // CTeamRoundTimer::m_nState
	_DATA_AMOUNT
};

class EntityDataProvider
{
private:
	EntityDataProvider() {}

public:
	static void Init();
	static void Destroy();

	template<typename T>
	inline T GetDataFromEntity(Entity entity, EntityDataType dataType) const
	{
		if (!entity.Exists() || !entity.GetEdict()->GetUnknown())
		{
			return (T) 0;
		}

		return (T) *((char*) entity.GetEdict()->GetUnknown()->GetBaseEntity() + _EntityOffsets[dataType].Offset);
	}

	template<typename T>
	inline void SetDataOfEntity(Entity entity, EntityDataType dataType, T data)
	{
		if (entity.Exists() && entity.GetEdict()->GetUnknown())
		{
			*((char*) entity.GetEdict()->GetUnknown()->GetBaseEntity() + _EntityOffsets[dataType].Offset) = data;
		}
	}

private:
	struct EntityData
	{
	public:
		EntityData(EntityDataType dataType, const char* entityClass, const char* entityAttr)
			: DataType(dataType), EntityClass(entityClass), EntityAttr(entityAttr) {}
	
	public:
		int Offset = -1;

		inline EntityDataType GetDataType() const
		{
			return DataType;
		}

		inline const char* GetEntityClass() const
		{
			return EntityClass;
		}

		inline const char* GetEntityAttr() const
		{
			return EntityAttr;
		}

	private:
		const EntityDataType DataType;
		const char* EntityClass;
		const char* EntityAttr;
	};

	std::vector<EntityData> _EntityOffsets =
	{
		{DATA_TEAM, "CBaseEntity", "m_iTeamNum"},
		{DATA_FLAG_OWNER, "CCaptureFlag", "m_hPrevOwner"},
		{DATA_FLAG_STATUS, "CCaptureFlag", "m_nFlagStatus"},
		{DATA_FLAG_DISABLED, "CCaptureFlag", "m_bDisabled"},
		{DATA_HEALTH, "CBaseObject", "m_iHealth"},
		{DATA_PLAYER_FOV, "CBasePlayer", "m_iFOV"},
		{DATA_PLAYER_CURRENTWEAPON, "CBaseCombatCharacter", "m_hActiveWeapon"},
		{DATA_PLAYER_ANGLE, "CBaseEntity", "m_angRotation"},
		{DATA_ROUND_TIMER_STATE, "CTeamRoundTimer", "m_nState"}
	};

	void UpdateOffsets();
};

extern EntityDataProvider* _EntityDataProvider;