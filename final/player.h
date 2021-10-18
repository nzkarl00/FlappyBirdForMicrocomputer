/** @file   player.h
    @author F. van Dorsser
    @author K. Moore
    @date   12 October 2021
    @brief  Implements player character movements
*/

#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_JUMPING_BOTTOM tinygl_point(2, 6)
#define PLAYER_JUMPING_TOP tinygl_point(0, 6)
#define PLAYER_NEUTRAL_BOTTOM tinygl_point(4, 6)
#define PLAYER_NEUTRAL_TOP tinygl_point(2, 6)
#define PLAYER_DUCKING_BOTTOM tinygl_point(4, 6)
#define PLAYER_DUCKING_TOP tinygl_point(2, 6)

#include "tinygl.h"
#include "navswitch.h"

typedef struct {
    tinygl_point_t top;
    tinygl_point_t bottom;
    char state;
} player_t;

player_t player_init(void);
void player_duck(player_t* player);
void player_jump(player_t* player);
void player_neutral(player_t* player);
void update_position(player_t* player);
#endif // PLAYER_H