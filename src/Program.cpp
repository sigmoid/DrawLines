#include <iostream>

#include "../Opal/Game.h"

#include "Gamestate/DrawState.h"
#include "../Opal/Graphics/Camera.h"

int main()
{
    Opal::Game game;

    game.Init(1920, 1080, "DrawLines", Opal::RendererType::VULKAN);

    game.Resize(1920/2, 1080/2);

    game.PushState<DrawState>();

    while(!game.ShouldEnd())
    {
        game.Tick();
    }
}