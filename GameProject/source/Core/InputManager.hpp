#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include "../Singleton.hpp"
#include <3ds.h>

class InputManager : public Singleton<InputManager>
{
    private:
        u32 keyDown;
    public:
        InputManager() = default;
        ~InputManager() = default;
        void Update(float dt);
        void CleanUp();

        u32 GetKeyDown() const { return keyDown; }
        
};



#endif