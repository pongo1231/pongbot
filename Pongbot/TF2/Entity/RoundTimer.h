#include "Entity.h"

class RoundTimer : public Entity
{
public:
    RoundTimer() {}
    RoundTimer(edict_t* edict) : Entity(edict) {}
    RoundTimer(Entity entity) : Entity(entity) {}

public:
    int GetRoundTimerStatus() const;
};