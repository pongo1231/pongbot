#include <stdafx.h>
#include "RoundTimer.h"
#include "EntityDataProvider.h"

int RoundTimer::GetRoundTimerStatus() const
{
    if (!Exists())
    {
        return -1;
    }

    return _EntityDataProvider->GetDataFromEntity<int>(*this, DATA_ROUND_TIMER_STATE);
}