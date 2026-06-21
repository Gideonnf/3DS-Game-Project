#include "Timer.hpp"

Timer::Timer()
{
    prevTime = osGetTime();
    deltaTime = 0.0f;
}

void Timer::Tick()
{
    u64 currTime = osGetTime();

    deltaTime = (currTime - prevTime) / 1000.0f;

    if (deltaTime > 0.1f)
        deltaTime = 0.1f; // Cap deltaTime to avoid large jumps

    prevTime = currTime;
}