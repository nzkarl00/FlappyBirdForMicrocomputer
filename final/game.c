#include <stdint.h>
#include <stdbool.h>
#include "display.h"
#include "ledmat.h"
#include "system.h"
#include "pio.h"
#include "navswitch.h"
#include "pacer.h"


int main(void) {
    system_init ();
        navswitch_init ();
        pacer_init (500);
        display_init ();
        pio_output_low(LEDMAT_ROW7_PIO);
        while (1)
        {
	    pacer_wait ();
            navswitch_update ();

            if (navswitch_push_event_p (NAVSWITCH_EAST)) {
                pio_output_high(LEDMAT_COL2_PIO);
                pio_output_high(LEDMAT_COL3_PIO);
                pio_output_low(LEDMAT_COL4_PIO);
            } else if (navswitch_push_event_p (NAVSWITCH_WEST)) {
                pio_output_low(LEDMAT_COL2_PIO);
                pio_output_low(LEDMAT_COL3_PIO);
                pio_output_high(LEDMAT_COL4_PIO);
            } else {
                pio_output_high(LEDMAT_COL2_PIO);
                pio_output_low(LEDMAT_COL3_PIO);
                pio_output_low(LEDMAT_COL4_PIO);
            }
        }
}
