#pragma once
#include "defines.h"

struct game;

typedef struct application_config
{
    i16 start_posX; //Windows starting pos X axis, if applicable
    i16 start_posY; //Windows starting pos Y axis, if applicable
    i16 start_width; //Windows starting width, if applicable
    i16 start_height; //Windows starting height, if applicable
    //Application Name used in Windowing
    char* name; 
}application_config;

FAPI b8 application_create(struct game* game_inst );
FAPI b8 application_run();
