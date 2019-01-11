#include "BotTask.h"
#include "BotVisibles.h"
#include "Util.h"
#include "TFClassInfoProvider.h"
#include "ConVarHolder.h"

Bot *_Bot;
Vector _BotTargetPos;
Vector _BotTargetLookAt;
bool _IsBotViewAngleOverriden;
int _BotPressedButtons;
WeaponSlot _WeaponSlot;

bool BotTask::OnThink()
{
	_IsBotViewAngleOverriden = false;
	_BotPressedButtons = 0;

	bool taskResult = _OnThink();

	if (!_IsBotViewAngleOverriden)
		_ShootAtBadGuys();

	// Avoid jittering around when standing still
	if (Util::DistanceToNoZ(_Bot->GetPos(), _BotTargetPos) < _ConVarHolder->CVarBotMovementIgnoreRadius->GetFloat())
		_Bot->SetMovement(Vector2D());
	else
		_Bot->SetMovement(Util::GetIdealMoveSpeedsToPos(_Bot, _BotTargetPos));
	_Bot->SetViewAngle(Util::GetLookAtAngleForPos(_Bot, _BotTargetLookAt));
	_Bot->SetPressedButtons(_BotPressedButtons);
	_Bot->SetSelectedWeapon(_WeaponSlot);

	return taskResult;
}

void BotTask::_ShootAtBadGuys()
{
	BotVisibleTarget *enemyTarget = _Bot->GetBotVisibles()->GetMostImportantTarget();
	if (enemyTarget)
	{
		Vector targetPos = enemyTarget->Pos;

		_BotTargetLookAt = targetPos;
		_BotPressedButtons |= IN_ATTACK;

		_WeaponSlot = _Bot->GetIdealWeaponForRange(targetPos.DistTo(_Bot->GetPos()));
	}
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

void BotTask::_SetBotWeaponSlot(WeaponSlot weaponSlot)
{
	_WeaponSlot = weaponSlot;
}

Bot *BotTask::_GetBot() const
{
	return _Bot;
}