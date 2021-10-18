#include <avr/io.h>
#include "pacer1.h"

static uint16_t counter_threshold;

/* Initialise the pacer module.  */
void pacer_init (uint16_t pacer_frequency)
{
    /* TODO: initialise timer/counter peripheral the
       same way as in lab2/lab2-ex2/timer.c but also calculate 
       the timer/counter value from the pacer frequency */
    TCCR1A = 0x00;
    TCCR1B = 0x05;
    TCCR1C = 0x00;

    counter_threshold = PRESCALED_CPU_F / pacer_frequency;
}


/* Pace a while loop.  */
void pacer_wait (void)
{

    /* TODO: Implement the same way as the timer_delay () function 
       lab2-except reset TCNT1 after the while loop. */
    while (TCNT1 < counter_threshold) {
        continue;
    }
    TCNT1 = 0;
}
