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



void reset_display_state(tinygl_pixel_value_t array[TINYGL_WIDTH][TINYGL_HEIGHT])
{
    for (tinygl_coord_t row = 0; row < TINYGL_HEIGHT; row++) {
        for (tinygl_coord_t col = 0; col < TINYGL_WIDTH; col++) {
            array[row][col] = 0;
        }
    }
}


#define PACER_RATE 5000
#define MESSAGE_RATE 10


// Mock-up of display update logic

int main(void)
{
    // call initialisation funcs
    system_init ();
    navswitch_init ();
    pacer_init (500);
    display_init ();
    game_state_t currentState = PLAYING_GAME; //TODO START GAME CONDITION
    static uint8_t highScore = 0;
    uint8_t currentScore = 0;
    char scoreString[3];

    tinygl_pixel_value_t displayState [TINYGL_WIDTH][TINYGL_HEIGHT] = {0};
    player_t playerCharacter = player_init();
    obstacle_t upperObstacle = upper_obstacle_init();
    obstacle_t lowerObstacle = lower_obstacle_init();
    obstacle_t obstacles[] = {upperObstacle, lowerObstacle};
    obstacle_t currentObstacle = obstacles[rand() & 1];
    int8_t tickCounter = 0;

    tinygl_init(PACER_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    while(1) {
        pacer_wait ();
        
        switch(currentState) {
            
            case PLAYING_GAME:
                if (tickCounter >= 5) {
                    reset_display_state(displayState);
                    navswitch_update ();
                    update_position(&playerCharacter);
                    for (tinygl_coord_t col = playerCharacter.top.x; col < (playerCharacter.bottom.x); col++) {
                        displayState[col][playerCharacter.top.y] = 1;
                    }
                    advance_obstacle(&currentObstacle);
                    if (currentObstacle.top.y >= TINYGL_HEIGHT) {
                        reset_obstacle(&currentObstacle);
                        currentObstacle = obstacles[rand() & 1];
                    }
                    currentScore++;
                    for (tinygl_coord_t col = currentObstacle.top.x; col < (currentObstacle.bottom.x); col++) {
                        displayState[col][currentObstacle.top.y] = 1;
                    }
                    
                    for (tinygl_coord_t row = 0; row < TINYGL_HEIGHT; row++) {
                        for (tinygl_coord_t col = 0; col < TINYGL_WIDTH; col++) {
                            display_pixel_set(row, col, displayState[row][col]);
                        }
                    }
                    if (currentObstacle.type == playerCharacter.state) {
                        currentState = GAME_OVER;
                    }
                }
                tickCounter++;
                tinygl_update(); 
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
