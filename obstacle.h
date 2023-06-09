/** @file   obstacle.h
    @author F. van Dorsser
    @author K. Moore
    @date   12 October 2021
    @brief  Implements game obstacles
*/

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdbool.h>
#include "tinygl.h"

/* Define initial obstacle display coords */
#define U_OBSTACLE_TOP tinygl_point(0, 0)
#define U_OBSTACLE_BOT tinygl_point(1, 0)

#define L_OBSTACLE_TOP tinygl_point(3, 0)
#define L_OBSTACLE_BOT tinygl_point(4, 0)

/* Define an obstacle (a line specified by two coordinates). 
   Two varieties: an upper type, which must be ducked under,
   and a lower, which must be jumped over. */
typedef struct {
    tinygl_point_t top;
    tinygl_point_t bottom;
    char type;            // 'U' for upper, 'L' for lower
} obstacle_t;

/* Create a new upper obstacle */
obstacle_t upper_obstacle_init(void);

/* Create a new lower obstacle */
obstacle_t lower_obstacle_init(void);

/* Advance an obstacle's position */
void advance_obstacle(obstacle_t* obstacle);

/* Reset an obstacle's position to the initial points */
void reset_obstacle(obstacle_t* obstacle);

/* Randomly select a new obstacle from the obstacles array */
obstacle_t get_new_obstacle(obstacle_t obstacles[]);

/* Update obstacle position and return a new one from the pool if it reaches the end of the display */
void update_obstacle(obstacle_t* obstacle, obstacle_t obstacles []);

/* Check to see if an obsacle is at the end of the display */
bool obstacle_at_end(obstacle_t* obstacle);

#endif // OBSTACLE_H