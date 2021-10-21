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

#include "tinygl.h"
#include "navswitch.h"

typedef struct {
    tinygl_point_t top;
    tinygl_point_t bottom;
    char state;
} player_t;

/* Initialises a player in the neutral state */
player_t player_init(void);

/* Returns the player character to the ground, allowing them to avoid high obstacles */
void player_fall(player_t* player);

/* Raises the player character so they can clear low obstacles */
void player_jump(player_t* player);

/* Checks if the user is inputting a jump command on the navswitch */
void update_position(player_t* player);

#endif // PLAYER_H