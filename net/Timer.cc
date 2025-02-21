#include "Timer.h"

using namespace net;

void Timer::restart(Timestamp now)
{
    if (repeat_)
    {
        expiration_ = addTime(now, interval_);
    }
    else
    {
        expiration_ = 0;
    }
}
