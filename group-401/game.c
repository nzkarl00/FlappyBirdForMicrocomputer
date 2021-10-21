/** @file   game.c
    @author F. van Dorsser
    @author K. Moore
    @date   18 October 2021
    @brief  Main loop for the game and game logic
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "display.h"
#include "ledmat.h"
#include "system.h"
#include "pio.h"
#include "navswitch.h"
#include "pacer1.h"
#include "tinygl.h"
#include "obstacle.h"
#include "player.h"
#include "../fonts/font5x7_1.h"


#define PACER_RATE 500
#define MESSAGE_RATE 20
#define GAME_UPDATE_RATE 100
#define PLAYER_BLINK_RATE 5
#define PLAYER_GRAVITY_TRIGGER 67
#define INIT_OBSTACLE_RATE 3
#define MAX_OBSTACLE_RATE 10
#define MAX_OBSTACLE_ADVANCES TINYGL_HEIGHT
#define PLAYER_POINTS_PER_SPEED_INCREASE 5
#define MAX_MESSAGE_LEN TINYGL_MESSAGE_SIZE
#define GAME_OVER_PERIOD 8000
#define COLLISION_DETECTED (currentObstacle.top.y == TINYGL_HEIGHT - 1) && \
                            (playerCharacter.state == currentObstacle.type)


typedef enum {
    START_GAME, PLAYING_GAME, GAME_OVER
} game_state_t;


int main(void)
{
    system_init();
    navswitch_init();
    pacer_init(PACER_RATE);
    display_init();
    tinygl_init(PACER_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);

    game_state_t currentState = START_GAME;
    uint8_t highScore = 0;
    uint8_t currentScore = 0;
    const char welcomeMessage[MAX_MESSAGE_LEN] = "Push navswitch to start playing";
    const char gameOverString[MAX_MESSAGE_LEN] = "Score: %d. High score: %d.    ";
    const char restartMessage[MAX_MESSAGE_LEN] = "Push navswitch to restart.";
    char gameOverMessage[MAX_MESSAGE_LEN];
    tinygl_clear();
    tinygl_text(welcomeMessage);

    player_t playerCharacter = player_init();
    obstacle_t upperObstacle = upper_obstacle_init();
    obstacle_t lowerObstacle = lower_obstacle_init();
    obstacle_t obstacles[] = {upperObstacle, lowerObstacle};
    obstacle_t currentObstacle = get_new_obstacle(obstacles);
    uint8_t obstacleRate = INIT_OBSTACLE_RATE;

    uint8_t playingTick = 0;
    uint8_t playerRefreshTick = 0;
    uint8_t playerAirtime = 0;
    uint8_t obstacleTick = 0;
    uint8_t obstacleAdvances = 0;
    uint16_t gameOverTick = 0;

    while(1) {
        pacer_wait ();
        
        switch(currentState) {
            
            case PLAYING_GAME:

                if (playingTick >= PACER_RATE / GAME_UPDATE_RATE) { //Update the player position every 5 ticks to match implicit display refresh rate
                    playingTick = 0;
                    display_clear();
                    navswitch_update();
                    update_position(&playerCharacter);

                    if (playerCharacter.state == 'U') {
                        playerAirtime++;
                    } else {
                        playerAirtime = 0;
                    }

                    if (playerAirtime >= PLAYER_GRAVITY_TRIGGER) { //Causes the player character to fall if they have been in the air for too long
                        player_fall(&playerCharacter);
                        playerAirtime = 0;
                    }
                }

                if (playerRefreshTick >= PACER_RATE / PLAYER_BLINK_RATE) {
                    playerRefreshTick = 0;
                    tinygl_draw_line(playerCharacter.top, playerCharacter.bottom, 1);
                }
                
                tinygl_draw_line(currentObstacle.top, currentObstacle.bottom, 1);

                if (COLLISION_DETECTED) { //Game end condition met
                    currentState = GAME_OVER;
                    display_clear();
                } else if (obstacleAdvances == MAX_OBSTACLE_ADVANCES) { //Game end condition not met
                    currentScore++;
                    obstacleAdvances = 0;
                }

                if (obstacleTick >= PACER_RATE / obstacleRate) {
                    obstacleTick = 0;
                    update_obstacle(&currentObstacle, obstacles);
                    obstacleAdvances++;
                }

                if (obstacleRate < MAX_OBSTACLE_RATE) {
                    obstacleRate = INIT_OBSTACLE_RATE + currentScore / PLAYER_POINTS_PER_SPEED_INCREASE;
                }

                playingTick++;
                playerRefreshTick++;
                obstacleTick++;
                display_update();
                break;

            case GAME_OVER:

                if (currentScore > highScore) {
                    highScore = currentScore;
                }
                if (gameOverTick == 0) {
                    gameOverMessage[0] = 0;
                    sprintf(gameOverMessage, gameOverString, currentScore, highScore);
                    tinygl_clear();
                    tinygl_text(gameOverMessage);
                }

                tinygl_update();
                
                gameOverTick++;

                if(gameOverTick >= GAME_OVER_PERIOD) { //Post game reset and instrucitonal message
                    currentState = START_GAME;
                    currentScore = 0;
                    playerCharacter = player_init();
                    reset_obstacle(&currentObstacle);
                    currentObstacle = get_new_obstacle(obstacles);
                    playingTick = 0;
                    playerRefreshTick = 0;
                    obstacleTick = 0;
                    obstacleRate = INIT_OBSTACLE_RATE;
                    obstacleAdvances = 0;
                    gameOverTick = 0;
                    tinygl_clear();
                    tinygl_text(restartMessage);
                }

                break;
                
            case START_GAME:
                
                tinygl_update();

                navswitch_update();
                if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                    currentState = PLAYING_GAME;
                }

                break;
        }
    }
}

