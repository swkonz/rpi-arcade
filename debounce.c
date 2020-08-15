#include "debounce.h"
#include "gpio.h"
#include "timer.h"

/* Constants */
#define NUM_PINS            54

/* Static Members */
static unsigned last_access_tick[NUM_PINS] = {0};

void debounce_init (unsigned pin)
{
    last_access_tick[pin] = timer_get_ticks();
}

bool debounce (unsigned pin)
{
    unsigned cur_ticks = timer_get_ticks ();
    if (cur_ticks - last_access_tick[pin] < 1000)
        return false;
    else 
        {
            last_access_tick[pin] = cur_ticks;
            return true;
        }
}
