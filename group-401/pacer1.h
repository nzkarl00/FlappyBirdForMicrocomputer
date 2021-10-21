/** @file   pacer1.h
    @author F. van Dorsser
    @author K. Moore
    @date   12 October 2021
    @brief  Simple pacer for pacing main game loop
*/

#ifndef PACER1_H
#define PACER1_H

#include "system.h"

#define PRESCALER 1024
#define PRESCALED_CPU_F (F_CPU / PRESCALER)

/* Initialise the pacer module */
void pacer_init (uint16_t pacer_frequency);

/* Pace a while loop */
void pacer_wait (void);

#endif //PACER1_H
