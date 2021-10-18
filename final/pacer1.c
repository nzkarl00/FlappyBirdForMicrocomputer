#include <avr/io.h>
#include "pacer1.h"

static uint16_t counter_threshold;

/* Initialise the pacer module.  */
void pacer_init (uint16_t pacer_frequency)
{
    TCCR1A = 0x00;
    TCCR1B = 0x05;
    TCCR1C = 0x00;

    counter_threshold = PRESCALED_CPU_F / pacer_frequency;
}


/* Pace a while loop.  */
void pacer_wait (void)
{
    while (TCNT1 < counter_threshold) {
        continue;
    }
    TCNT1 = 0;
}
