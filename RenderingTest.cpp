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
    game.ConstructConsole(160, 100);

    while (true)
    {
        game.Draw(1, 1);
    }

    std::cout << "Hello World!\n";
}