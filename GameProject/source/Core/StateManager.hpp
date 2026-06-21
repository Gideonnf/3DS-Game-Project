#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H
#include "../Singleton.hpp"


class StateManager : public Singleton<StateManager>
{
    public:
        StateManager() = default;
        ~StateManager() = default;
        void Update(float dt);
        void CleanUp();
};


#endif