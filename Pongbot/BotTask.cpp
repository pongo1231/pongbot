#include "Bot.h"
#include "BotTask.h"
#include "BotVisibles.h"
#include "Util.h"
#include "TFClassInfoProvider.h"

Bot *_Bot;
Vector _BotTargetPos;
Vector _BotTargetLookAt;
bool _IsBotViewAngleOverriden;
int _BotPressedButtons;

ConVar _CVarPrimaryWeaponDist("pongbot_bot_primwepdist", "600.0", 0, "Max distance where bots use their primary weapon");
ConVar _CVarMeleeWeaponDist("pongbot_bot_meleewepdist", "300.0", 0, "Max distance where bots use their melee weapon");

bool BotTask::OnThink()
{
	_BotTargetPos.Zero();
	_BotTargetLookAt.Zero();
	_IsBotViewAngleOverriden = false;
	_BotPressedButtons = 0;

	bool taskResult = _OnThink();

	if (!_IsBotViewAngleOverriden)
		_ShootAtBadGuys();

	_Bot->SetViewAngle(Util::GetLookAtAngleForPos(_Bot, _BotTargetLookAt));
	_Bot->SetMovement(Util::GetIdealMoveSpeedsToPos(_Bot, _BotTargetPos));
	_Bot->SetPressedButtons(_BotPressedButtons);

	return taskResult;
}

void BotTask::_ShootAtBadGuys()
{
	BotVisibleTarget *enemyTarget = _Bot->GetBotVisibles()->GetMostImportantTarget();
	if (enemyTarget)
	{
		_BotTargetLookAt = enemyTarget->Pos;
		_BotPressedButtons |= IN_ATTACK;

		_ChooseBestWeaponForDistance(enemyTarget->Pos.DistTo(_Bot->GetPos()));
	}
}

void BotTask::_ChooseBestWeaponForDistance(float distance)
{
	bool primarySecondarySwap = _TFClassInfoProvider->GetClassInfo(_Bot->GetClass()).PrimaryWeaponSwap;
	WeaponSlot weaponSlot;
	if (distance < _CVarMeleeWeaponDist.GetFloat())
		weaponSlot = WEAPON_MELEE;
	else if (distance < _CVarPrimaryWeaponDist.GetFloat())
		weaponSlot = primarySecondarySwap ? WEAPON_SECONDARY : WEAPON_PRIMARY;
	else
		weaponSlot = primarySecondarySwap ? WEAPON_PRIMARY : WEAPON_SECONDARY;

	_Bot->SetSelectedWeapon(weaponSlot);
}

void BotTask::_BotMoveTo(Vector pos)
{
	_BotTargetPos = pos;
}

void BotTask::_SetBotLookAt(Vector pos)
{
	_BotTargetLookAt = pos;
}

void BotTask::_OverrideBotViewAngle()
{
	_IsBotViewAngleOverriden = true;
}

void BotTask::_AddBotPressedButton(int button)
{
	_BotPressedButtons |= button;
}

Bot *BotTask::_GetBot() const
{
	return _Bot;
}