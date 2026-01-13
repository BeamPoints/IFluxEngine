#include <entry.h>
#include <Core/defines.h>
#include "game.h"

//defines the function to create a game
b8 create_game(game* out_game)
{
    application_config config; application_config app_config;
    config.start_posX = 100;
    config.start_posY = 100;
    config.start_width = 1280;
    config.start_height = 720;
    config.name = "I Flux Engine";

    out_game->app_config = config;
    out_game->update = game_update;
    out_game->renderer = game_render;
    out_game->initalize = game_initalize;
    out_game->onresize = Game_on_resize;

    return True;
}
