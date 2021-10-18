/** @file   player.c
    @author F. van Dorsser
    @author K. Moore
    @date   12 October 2021
    @brief  Implements player movements
*/

#include "player.h"
#include "tinygl.h"
#include "navswitch.h"

player_t player_init()
{
    player_t player = {PLAYER_NEUTRAL_TOP, PLAYER_NEUTRAL_BOTTOM, 'N'};
    return player;
}

void player_jump(player_t* player) {
    player->top = PLAYER_JUMPING_TOP;
    player->bottom = PLAYER_JUMPING_BOTTOM;
    player->state = 'U';
}

void player_neutral(player_t* player) {
    player->top = PLAYER_NEUTRAL_TOP;
    player->bottom = PLAYER_NEUTRAL_BOTTOM;
    player->state = 'L';
}

void player_duck(player_t* player) {
    player->top = PLAYER_DUCKING_TOP;
    player->bottom = PLAYER_DUCKING_BOTTOM;
    player->state = 'L';
}

void update_position(player_t* player) {
    if (navswitch_push_event_p (NAVSWITCH_EAST)) {
        player_duck(player);
    } else if (navswitch_push_event_p (NAVSWITCH_WEST)) {
        player_jump(player);
    }
}