#include "Timer.h"

using namespace std::chrono;

Timer::Timer()
{
    reset();
}

void Timer::reset()
{
    start = steady_clock::now();
    end = start;
    dtime = 0.0f;
}

void Timer::update()
{
    end = steady_clock::now();
    dtime = duration<float>(end - start).count();
    start = end;
}

float Timer::getDelta() const
{
    return dtime;
}
