#include <stdafx.h>
#include "BotTask.h"
#include "../../Visibles/BotVisibles.h"
#include "../../../Util.h"
#include "../../../TF2/Class/TFClassInfoProvider.h"
#include "../../../ConVarHolder.h"

bool BotTask::OnThink()
{
	_BotTargetPos.Zero();
	_BotTargetAngle = {0.f, 0.f, 0.f};
	_IsBotViewAngleOverriden = false;
	_BotPressedButtons = 0;

	bool taskResult = true;
	if (!_AbortTask)
	{
		taskResult = _OnThink();
	}
	
	if (taskResult)
	{
		_OnStop();
	}

	if (!_IsBotViewAngleOverriden)
	{
		_ShootAtBadGuys();
	}

	if (_DoStuckPosPanicHandling)
	{
		_CheckIfStuckInPos();
	}

	// Avoid jittering around when supposed to stand still
	if (!_BotTargetPos.IsValid() || _BotTargetPos.IsZero() || Util::DistanceToNoZ(_Bot->GetPos(), _BotTargetPos) < _ConVarHolder->CVarBotMovementIgnoreRadius->GetFloat())
	{
		_Bot->SetMovement(Vector2D(0.f, 0.f));
	}
	else
	{
		_Bot->SetMovement(Util::GetIdealMoveSpeedsToPos(_Bot, _BotTargetPos));
	}

	if (!_BotTargetLookAt.IsZero())
	{
		_Bot->SetViewAngle(Util::GetLookAtAngleForPos(_Bot, _BotTargetLookAt));
	}
	else if (_BotTargetAngle != QAngle(0.f, 0.f, 0.f))
	{
		_Bot->SetViewAngle(_BotTargetAngle);
	}
	else
	{
		_Bot->SetViewAngle({0.f, 0.f, 0.f});
	}
	
	_Bot->SetPressedButtons(_BotPressedButtons);
	_Bot->SetSelectedWeapon(_WeaponSlot);

	return taskResult;
}

void BotTask::_ShootAtBadGuys()
{
	BotVisibleTarget enemyTarget = _Bot->GetBotVisibles()->GetMostImportantTarget();
	if (enemyTarget.IsValid())
	{
		Vector targetPos = enemyTarget.GetPos();

		_BotTargetLookAt = targetPos;
		_BotPressedButtons |= IN_ATTACK;

		_WeaponSlot = _Bot->GetIdealWeaponForRange(targetPos.DistTo(_Bot->GetPos()));
	}
}

void BotTask::_CheckIfStuckInPos()
{
	Vector currentPos = _Bot->GetPos();

	if (Util::DistanceToNoZ(currentPos, _LastPos) < _ConVarHolder->CVarBotPosStuckPanicRange->GetFloat())
	{
		_PosStuckTime++;

		float panicStuckTime = _ConVarHolder->CVarBotPosStuckPanicTime->GetFloat();
		if (_PosStuckTime > panicStuckTime + 50.f)
		{
			_PosStuckTime = 0;
			_PosStuckTries++;

			if (_PosStuckTries > _ConVarHolder->CVarBotPosStuckPanicTries->GetInt() - 1)
			{
				_PosStuckTries = 0;
				_OnBotDefinitelyStuck();
			}
			else
			{
				_OnBotStuckPanic();
			}
		}
		else if (_PosStuckTime > panicStuckTime)
		{
			_AddBotPressedButton(IN_JUMP);
			_AddBotPressedButton(IN_DUCK);
			_Bot->ExecClientCommand("voicemenu 2 5");
		}
	}
	else
	{
		_PosStuckTime = 0;
		_PosStuckTries = 0;
		_LastPos = currentPos;
	}
}