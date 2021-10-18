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


int main(void) {
    system_init ();
        navswitch_init ();
        pacer_init (500);
        display_init ();
        pio_output_low(LEDMAT_ROW7_PIO);
        while (1)
        {
	    pacer_wait ();
            navswitch_update ();

            if (navswitch_push_event_p (NAVSWITCH_EAST)) {
                pio_output_high(LEDMAT_COL2_PIO);
                pio_output_high(LEDMAT_COL3_PIO);
                pio_output_low(LEDMAT_COL4_PIO);
            } else if (navswitch_push_event_p (NAVSWITCH_WEST)) {
                pio_output_low(LEDMAT_COL2_PIO);
                pio_output_low(LEDMAT_COL3_PIO);
                pio_output_high(LEDMAT_COL4_PIO);
            } else {
                pio_output_high(LEDMAT_COL2_PIO);
                pio_output_low(LEDMAT_COL3_PIO);
                pio_output_low(LEDMAT_COL4_PIO);
            }
        }
}


typedef enum {
    START_GAME, PLAYING_GAME, GAME_OVER
} game_state_t;



void reset_display_state(tinygl_pixel_value_t* array, uint8_t length)
{
    for (uint8_t index = 0; index < length; index++) {
        *(array + index) = 0;
    }
}





// Mock-up of display update logic

int main(void)
{
    // call initialisation funcs

    game_state_t currentState = START_GAME;
    static uint16_t highScore = 0;
    uint16_t currentScore = 0;

    tinygl_pixel_value_t displayState[TINYGL_WIDTH][TINYGL_HEIGHT] = {0};

    obstacle_t upperObstacle = upper_obstacle_init();
    obstacle_t lowerObstacle = lower_obstacle_init();
    obstacle_t obstacles[] = {upperObstacle, lowerObstacle};
    obstacle_t currentObstacle = obstacles[rand() & 1];

    while(1) {
        pacer_wait ();
        
        switch(currentState) {
            
            case PLAYING_GAME:
                if (/* tick % (PACER_RATE / GAME_UPDATE_RATE) == 0 */) { //GAME_UPDATE_RATE = 1/5 PACER_RATE?
                    reset_display_state(displayState, (TINYGL_WIDTH * TINYGL_HEIGHT));
                    navswitch_update ();
                    //update player position

                    advance_obstacle(&currentObstacle);
                    if (currentObstacle.top.y >= TINYGL_HEIGHT) {
                        reset_obstacle(&currentObstacle);
                        currentObstacle = obstacles[rand() & 1];
                    }
                for (tinygl_coord_t col = currentObstacle.top.x; col < (currentObstacle.bottom.x); col++) {
                    displayState[col][currentObstacle.top.y] = 1;
                }
                if (/* obstacle intersects with player */) {
                    currentState = GAME_OVER;
                }

                tinygl_update(); 
                break;



            case START_GAME:
                // display welcome message; navswitch push to start game?
        }

    }



}
