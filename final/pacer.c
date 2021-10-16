#include <avr/io.h>
#include "pacer.h"

static uint16_t pacer_period;

/* Initialise the pacer module.  */
void pacer_init (uint16_t pacer_frequency)
{
    TCCR1A = 0x00;
    TCCR1B = 0x05;
    TCCR1C = 0x00;
    pacer_period = pacer_frequency * 400;
}


/* Pace a while loop.  */
void pacer_wait (void)
{
    while (TCNT1 < pacer_period) {
        continue;   
    }
    TCNT1 = 0;
}
