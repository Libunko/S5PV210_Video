#ifndef __RETRO_SNAKER_H
#define __RETRO_SNAKER_H

#include <stdlib.h>


#include "../inc/draw_picture.h"
#include "../inc/process_ui.h"


#define UP          0
#define DOWN        1
#define LEFT        2
#define RIGHT       3
#define GAME_OVER   4
#define GAME_START  5
#define GAME_END    6

unsigned char direction;		//方向

unsigned char move(void);
void snaker_init(void);




#endif
