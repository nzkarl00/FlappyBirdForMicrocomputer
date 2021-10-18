#ifndef PACER1_H
#define PACER1_H

#include "system.h"

#define PRESCALED_CPU_F (F_CPU / 1024)

/* Initialise the pacer module.  */
void pacer_init (uint16_t pacer_frequency);


/* Pace a while loop.  */
void pacer_wait (void);

#endif //PACER1_H
