#pragma once
#include "Core/application.h"
#include "Core/logger.h"
#include "game_types.h"


// Externaly-defined funktion to Crate a Game;
extern b8 create_game(game* out_game);

// The  Main Entry Point of the Application 

int main(void)
{
    //request the game Instance form the Application
    game game_inst;
    if(!create_game(&game_inst))
    {
        FFATAL("Could not Create Game");
        return -1;
    }
    //Ensure the function pointer exist;
    if(!game_inst.renderer || !game_inst.update || !game_inst.initalize || !game_inst.onresize)
    {
        FFATAL("The Game's function pointers must be assinged");
        return -2;
    }
    //Init
    if(!application_create(&game_inst))
    {
        FINFO("Application failed to Create \n");
        return 1;
    }
    //Begin Game Loop
    if(!application_run())
    {
        FINFO("Application did not shutdown gracefully")
        return 2;
    }
    return 0;
}   