/** @file   obstacle.c
    @author F. van Dorsser
    @author K. Moore
    @date   12 October 2021
    @brief  Implements game obstacles
*/

#include "obstacle.h"


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
void advance_obstacle(obstacle_t obstacle)
{
    obstacle.top.y = (obstacle.top.y + 1) % TINYGL_HEIGHT;
    obstacle.bottom.y = (obstacle.bottom.y + 1) % TINYGL_HEIGHT;
}

#include "pacer.h"
#include "tinygl.h"

void main(void)
{
    pacer_init(1000);
    tinygl_init(1000);

    tinygl_draw_line(tinygl_point(1, 2), tinygl_point(3, 5), 1);

    while (1)
    {
        pacer_wait();
        tinygl_update();
    }
}