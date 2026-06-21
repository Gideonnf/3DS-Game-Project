#include "InputManager.hpp"

void InputManager::Update(float dt)
{
     //Scan all the inputs. This should be done once for each frame
     hidScanInput();

    //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
    keyDown = hidKeysDown();

}

void InputManager::CleanUp()
{
    // Clean up resources related to input management if necessary
}