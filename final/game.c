/** @file   player.h
    @author F. van Dorsser
    @author K. Moore
    @date   12 October 2021
    @brief  Main loop for the game and game logic
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "display.h"
#include "ledmat.h"
#include "system.h"
#include "pio.h"
#include "navswitch.h"
#include "pacer.h"
#include "tinygl.h"
#include "obstacle.h"
#include "uint8toa.h"
#include "player.h"
#include "../fonts/font5x7_1.h"


typedef enum {
    START_GAME, PLAYING_GAME, GAME_OVER
} game_state_t;



void reset_display_state(tinygl_pixel_value_t array[TINYGL_HEIGHT][TINYGL_WIDTH])
{
    for (tinygl_coord_t row = 0; row < TINYGL_HEIGHT; row++) {
        for (tinygl_coord_t col = 0; col < TINYGL_WIDTH; col++) {
            array[row][col] = 0;
        }
    }
}




// Mock-up of display update logic

int main(void)
{
    // call initialisation funcs
    system_init ();
    navswitch_init ();
    pacer_init (250);
    display_init ();
    game_state_t currentState = PLAYING_GAME; //TODO START GAME CONDITION
    static uint8_t highScore = 0;
    uint8_t currentScore = 0;
    char scoreString[3];

    tinygl_pixel_value_t displayState [TINYGL_HEIGHT][TINYGL_WIDTH] = {0};
    player_t playerCharacter = player_init();
    obstacle_t upperObstacle = upper_obstacle_init();
    obstacle_t lowerObstacle = lower_obstacle_init();
    obstacle_t obstacles[] = {upperObstacle, lowerObstacle};
    obstacle_t currentObstacle = obstacles[0];
    int8_t tickCounter = 0;
    int8_t playerRefresh = 0;
    int8_t obstacleMove = 0;
    while(1) {
        pacer_wait ();
        
        switch(currentState) {
            
            case PLAYING_GAME:
                if (tickCounter >= 5) { //Only updates the player position every 5 ticks to prevent ghosting
                    navswitch_update ();
                    update_position(&playerCharacter);
                    tinygl_clear();
                    currentScore++;  
                    tickCounter = 0;
                    if (playerRefresh >= 10) {
                        tinygl_draw_line(playerCharacter.top, playerCharacter.bottom, 1);
                        playerRefresh = 0;
                        obstacleMove++;
                    }
                    if (obstacleMove >= 10) {
                            advance_obstacle(&currentObstacle);
                            obstacleMove = 0;
                    }
                    playerRefresh++;
                }
                tinygl_draw_line(currentObstacle.top, currentObstacle.bottom, 1);
                display_update();
                tickCounter++;
                break;
            case GAME_OVER:
                if (currentScore > highScore) {
                    highScore = currentScore;
                }
                uint8toa(highScore, scoreString, false);
                tinygl_text(scoreString);
                tinygl_update();
                break;
                
            case START_GAME:
                // display welcome message; navswitch push to start game?
                break;
        }
    }
}
