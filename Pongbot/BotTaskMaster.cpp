#include "BotTaskMaster.h"
#include "Util.h"
#include "Bot.h"
#include "BotVisibles.h"
#include "TFClass.h"
#include "BotTaskMasterCollection.h"
#include <hlsdk/public/mathlib/mathlib.h>
#include <hlsdk/public/edict.h>
#include <hlsdk/game/shared/in_buttons.h>
#include <vector>

Bot *_Bot;
BotTaskMaster *_BotClassTaskMaster;

BotTaskMaster::BotTaskMaster(Bot *bot) {
	UpdateClass();
}

BotTaskMaster::~BotTaskMaster() {
	delete _BotClassTaskMaster;
}

void BotTaskMaster::UpdateClass() {
	switch (_Bot->GetClass()) {
	case SCOUT:
		_BotClassTaskMaster = new BotTaskMasterScout(_Bot);
	case SOLDIER:
		_BotClassTaskMaster = new BotTaskMasterSoldier(_Bot);
	case PYRO:
		_BotClassTaskMaster = new BotTaskMasterPyro(_Bot);
	case DEMO:
		_BotClassTaskMaster = new BotTaskMasterDemo(_Bot);
	case HEAVY:
		_BotClassTaskMaster = new BotTaskMasterHeavy(_Bot);
	case ENGI:
		_BotClassTaskMaster = new BotTaskMasterEngi(_Bot);
	case MED:
		_BotClassTaskMaster = new BotTaskMasterMed(_Bot);
	case SNIPER:
		_BotClassTaskMaster = new BotTaskMasterSniper(_Bot);
	case SPY:
		_BotClassTaskMaster = new BotTaskMasterSpy(_Bot);
	}
}

void BotTaskMaster::OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt) {
	if (_BotClassTaskMaster)
		_BotClassTaskMaster->OnThink(pressedButtons, movement, lookAt);

	if (!lookAt) {
		std::vector<edict_t*> visibleEdicts = _Bot->GetBotVisibles()->GetVisibleEdicts();
		if (visibleEdicts.size() > 0) {
			lookAt = new QAngle(Util::GetLookAtAngleForPos(_Bot, Util::GetEdictOrigin(
				visibleEdicts[Util::RandomInt(0, visibleEdicts.size() - 1)])));
			*pressedButtons |= IN_ATTACK;
		}
	}
}