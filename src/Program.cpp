#include <iostream>

#include "../Opal/Game.h"

#include "Gamestate/DrawState.h"

int main()
{
    Opal::Game game;

    game.Init(1920, 1080, "DrawLines", Opal::RendererType::VULKAN);

    game.PushState<DrawState>();

    while(!game.ShouldEnd())
    {
        game.Tick();
    }
}