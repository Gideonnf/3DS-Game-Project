#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include "../Singleton.hpp"

class InputManager : public Singleton<InputManager>
{
    public:
        InputManager() = default;
        ~InputManager() = default;
        void Update(float dt);
        void CleanUp();
};



#endif