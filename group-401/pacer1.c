/** @file   pacer1.c
    @author F. van Dorsser
    @author K. Moore
    @date   12 October 2021
    @brief  Simple pacer for pacing main game loop
*/

#include <avr/io.h>
#include "pacer1.h"

#if PRESCALER == 1
#define TCCR1B_INIT 0x01
#elif PRESCALER == 8
#define TCCR1B_INIT 0x02
#elif PRESCALER == 64
#define TCCR1B_INIT 0x03
#elif PRESCALER == 256
#define TCCR1B_INIT 0x04
#elif PRESCALER == 1024
#define TCCR1B_INIT 0x05
#else
#error Invalid PRESCALER
#endif

static uint16_t counter_threshold;

/* Initialise the pacer module */
void pacer_init (uint16_t pacer_frequency)
{
    TCCR1A = 0x00;
    TCCR1B = TCCR1B_INIT;
    TCCR1C = 0x00;

    counter_threshold = PRESCALED_CPU_F / pacer_frequency;
}

/* Pace a while loop */
void pacer_wait (void)
{
    while (TCNT1 < counter_threshold) {
        continue;
    }
    TCNT1 = 0;
}
