/** @file   player.h
    @author F. van Dorsser
    @author K. Moore
    @date   12 October 2021
    @brief  Implements pacer
*/

#ifndef PACER_H
#define PACER_H

#include "system.h"

#define PRESCALED_CPU_F (F_CPU / 1024)

/* Initialise the pacer module.  */
void pacer_init (uint16_t pacer_frequency);


/* Pace a while loop.  */
void pacer_wait (void);

#endif //PACER_H
