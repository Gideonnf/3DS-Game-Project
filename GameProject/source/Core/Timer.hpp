#ifndef TIMER_H
#define TIMER_H
#include <3ds.h>

class Timer
{
    private:
        u64 prevTime;
        float deltaTime;

    public:
        Timer();
        ~Timer() = default;
        void Tick();
        float GetDeltaTime() const { return deltaTime; }
};


#endif