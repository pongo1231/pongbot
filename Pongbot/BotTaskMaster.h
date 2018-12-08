#pragma once

class Bot;
class Vector2D;
class QAngle;

class BotTaskMaster {
public:
	virtual void OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt) = 0;
};

