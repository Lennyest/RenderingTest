#include <iostream>

#include "Engine.h"

class GameTest : public GameEngine
{
public:
    GameTest()
    {
        AppName = L"Game Engine Test";
    }
};

int main()
{
    GameTest game;
    game.ConstructConsole(256, 256);
    game.Start();
}