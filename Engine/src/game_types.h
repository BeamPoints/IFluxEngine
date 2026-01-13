#pragma once
#include "Core/application.h"


//Represents the basic Game state in a Game.
//Called for Creation by the Application;
typedef struct game
{
    // The Application Config
    application_config app_config;
    // Function Pointer to Game init Function
    b8 (*initalize)(struct game* game_inst);
    // Function Pointer to Game Update Function
    b8 (*update)(struct game* game_inst, f32 delta_time);
    // Function pointer to Game`s Renderer Function
    b8 (*renderer)(struct game* game_inst, f32 delta_time);
    // Function Pointer to handle Resize, if Applicable;
    void (*onresize)(struct game* game_inst, u32 width, u32 height);
    // Game-Specific game State. Crated and managed by the Game;
    void* state;
}game;
