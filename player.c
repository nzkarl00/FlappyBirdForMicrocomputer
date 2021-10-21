/** @file   player.c
    @author F. van Dorsser
    @author K. Moore
    @date   12 October 2021
    @brief  Implements player movements
*/

#include "player.h"
#include "tinygl.h"
#include "navswitch.h"

/* Initialises a player in the neutral state */
player_t player_init()
{
    player_t player = {PLAYER_NEUTRAL_TOP, PLAYER_NEUTRAL_BOTTOM, 'L'};
    return player;
}

/* Raises the player character so they can clear low obstacles */
void player_jump(player_t* player) 
{
    player->top = PLAYER_JUMPING_TOP;
    player->bottom = PLAYER_JUMPING_BOTTOM;
    player->state = 'U';
}

/* Returns the player character to the ground, allowing them to avoid high obstacles */
void player_fall(player_t* player) 
{
    player->top = PLAYER_NEUTRAL_TOP;
    player->bottom = PLAYER_NEUTRAL_BOTTOM;
    player->state = 'L';
}

/* Checks if the user is inputting a jump command on the navswitch */
void update_position(player_t* player) 
{
    if (navswitch_push_event_p (NAVSWITCH_WEST)) {
        player_jump(player);
    }
}