/** @file   player.h
    @author F. van Dorsser
    @author K. Moore
    @date   18 October 2021
    @brief  Main loop for the game and game logic
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"
#include "ledmat.h"
#include "system.h"
#include "pio.h"
#include "navswitch.h"
#include "pacer1.h"
#include "tinygl.h"
#include "obstacle.h"
#include "player.h"
#include "uint8toa.h"
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
#define SCORE_STR_LEN 4
#define FINAL_MESSAGE_LEN 36
#define GAME_OVER_PERIOD 10000
#define COLLISION_DETECTED (currentObstacle.top.y == TINYGL_HEIGHT - 1) && \
                            (playerCharacter.state == currentObstacle.type)


typedef enum {
    START_GAME, PLAYING_GAME, GAME_OVER
} game_state_t;


int main(void)
{
    // call initialisation funcs
    system_init();
    navswitch_init();
    pacer_init(PACER_RATE);
    display_init();
    tinygl_init(PACER_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);

    game_state_t currentState = START_GAME;
    static uint8_t highScore = 0;
    uint8_t currentScore = 0;
    const char welcomeMessage[] = "Welcome! Push navswitch to start";
    const char gameOverMessage[] = "Game over! Your high score is: ";
    char scoreString[SCORE_STR_LEN];
    char finalMessage[FINAL_MESSAGE_LEN];
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

                if (playingTick >= PACER_RATE / GAME_UPDATE_RATE) { //Only updates the player position every 5 ticks to prevent ghosting
                    playingTick = 0;
                    display_clear();
                    navswitch_update();
                    update_position(&playerCharacter);

                    if (playerCharacter.state == 'U') {
                        playerAirtime++;
                    } else {
                        playerAirtime = 0;
                    }

                    if (playerAirtime >= PLAYER_GRAVITY_TRIGGER) {
                        player_duck(&playerCharacter);
                        playerAirtime = 0;
                    }
                }

                if (playerRefreshTick >= PACER_RATE / PLAYER_BLINK_RATE) {
                    playerRefreshTick = 0;
                    tinygl_draw_line(playerCharacter.top, playerCharacter.bottom, 1);
                }
                
                tinygl_draw_line(currentObstacle.top, currentObstacle.bottom, 1);

                if (COLLISION_DETECTED) {
                    currentState = GAME_OVER;
                    display_clear();
                } else if (obstacleAdvances == MAX_OBSTACLE_ADVANCES) {
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
                    scoreString[0] = 0;
                    finalMessage[0] = 0;
                    uint8toa(highScore, scoreString, false);
                    strcpy(finalMessage, gameOverMessage);
                    strcat(finalMessage, scoreString);
                    tinygl_clear();
                    tinygl_text(finalMessage);
                }

                tinygl_update();
                
                gameOverTick++;

                if(gameOverTick >= GAME_OVER_PERIOD) {
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
                    tinygl_text(welcomeMessage);
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

