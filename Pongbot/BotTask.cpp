#include "Bot.h"
#include "BotTask.h"
#include "BotVisibles.h"
#include "Util.h"

Bot *_Bot;
Vector _BotTargetPos;
Vector _BotTargetLookAt;
bool _IsBotViewAngleOverriden;
int _BotPressedButtons;

bool BotTask::OnThink()
{
	_BotTargetPos.Zero();
	_BotTargetLookAt.Zero();
	_IsBotViewAngleOverriden = false;
	_BotPressedButtons = 0;

	bool taskResult = _OnThink();

	if (!_IsBotViewAngleOverriden)
	{
		BotVisibleTarget *enemyTarget = _Bot->GetBotVisibles()->GetMostImportantTarget();
		if (enemyTarget)
		{
			_BotTargetLookAt = enemyTarget->Pos;
			_BotPressedButtons |= IN_ATTACK;
		}
	}
	_Bot->SetViewAngle(Util::GetLookAtAngleForPos(_Bot, _BotTargetLookAt));
	_Bot->SetMovement(Util::GetIdealMoveSpeedsToPos(_Bot, _BotTargetPos));
	_Bot->SetPressedButtons(_BotPressedButtons);

	return taskResult;
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