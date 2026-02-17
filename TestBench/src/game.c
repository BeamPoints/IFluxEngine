#include "game.h"
#include <core/logger.h>

b8 game_initalize(game* game_inst)
{
    FINFO("Game initalized");
    return True;
}

b8 game_update(game* game_inst, f32 delta_time)
{
    return True;
}
b8 game_render(game* game_inst, f32 delta_time)
{
    return True;
}

void Game_on_resize(game* game_inst, u32 width, u32 height)
{

}