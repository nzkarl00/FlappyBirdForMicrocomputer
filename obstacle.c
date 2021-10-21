/** @file   obstacle.c
    @author F. van Dorsser
    @author K. Moore
    @date   12 October 2021
    @brief  Implements game obstacles
*/

#include <stdlib.h>
#include "obstacle.h"
#include "tinygl.h"

/* Create a new upper obstacle */
obstacle_t upper_obstacle_init(void)
{
    obstacle_t newObstacle = {U_OBSTACLE_TOP, U_OBSTACLE_BOT, 'U'};
    return newObstacle;
}

/* Create a new lower obstacle */
obstacle_t lower_obstacle_init(void)
{
    obstacle_t newObstacle = {L_OBSTACLE_TOP, L_OBSTACLE_BOT, 'L'};
    return newObstacle;
}

/* Advance an obstacle's position, resetting to the top of the screen 
   if outside bounds */
void advance_obstacle(obstacle_t* obstacle)
{
    obstacle->top.y += 1;
    obstacle->bottom.y += 1;
}

/* Reset an obstacle's position to the initial points */
void reset_obstacle(obstacle_t* obstacle)
{
    obstacle->top = obstacle->type == 'U' ? U_OBSTACLE_TOP : L_OBSTACLE_TOP;
    obstacle->bottom = obstacle->type == 'U' ? U_OBSTACLE_BOT : L_OBSTACLE_BOT;
}

/* Randomly select a new obstacle from the obstacles array */
obstacle_t get_new_obstacle(obstacle_t obstacles[])
{
    obstacle_t newObstacle = obstacles[rand() & 1];
    return newObstacle;
}

/* Update the current obstacle's position, selecting a new one if it reaches the end of the display */
void update_obstacle(obstacle_t* currentObstacle, obstacle_t obstacles [])
{
    advance_obstacle(currentObstacle);
    if (currentObstacle->top.y >= TINYGL_HEIGHT) {
        reset_obstacle(currentObstacle);
        *currentObstacle = get_new_obstacle(obstacles);
    }    
}

/* Check to see if an obsacle is at the end of the display */
bool obstacle_at_end(obstacle_t* obstacle) {
    if (obstacle->top.y >= TINYGL_HEIGHT) {
        return true;
    } else {
        return false;
    }
}