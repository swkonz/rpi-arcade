#include "buttons.h"
#include "constants.h"
#include "debounce.h"
#include "gpio.h"
#include "gpioextra.h"
#include "gpio_interrupts.h"
#include "interrupts.h"
#include "stdbool.h"

/* Setup all interrupts */
void interrupts_setup ()
{
    gpio_init ();

    /* set all buttons to input */
    gpio_set_input (BUTTON_A);
    gpio_set_input (BUTTON_B);
    gpio_set_input (BUTTON_LEFT);
    gpio_set_input (BUTTON_RIGHT);
    gpio_set_input (BUTTON_UP);
    gpio_set_input (BUTTON_DOWN);

    /* set internal pullup */
    // gpio_set_pullup (BUTTON_A);
    // gpio_set_pullup (BUTTON_B);
    // gpio_set_pullup (BUTTON_LEFT);
    // gpio_set_pullup (BUTTON_RIGHT);
    // gpio_set_pullup (BUTTON_UP);
    // gpio_set_pullup (BUTTON_DOWN);

    /* initialize debounce */
    debounce_init (BUTTON_A);
    debounce_init (BUTTON_B);
    debounce_init (BUTTON_LEFT);
    debounce_init (BUTTON_RIGHT);
    debounce_init (BUTTON_UP);
    debounce_init (BUTTON_DOWN);

    /* Interrupts Init */
    interrupts_init();
    gpio_interrupts_init();

    /* Enable event detection on each pin */
    gpio_enable_event_detection (BUTTON_B, GPIO_DETECT_RISING_EDGE);
    gpio_enable_event_detection (BUTTON_A, GPIO_DETECT_RISING_EDGE);
    gpio_enable_event_detection (BUTTON_LEFT, GPIO_DETECT_RISING_EDGE);
    gpio_enable_event_detection (BUTTON_RIGHT, GPIO_DETECT_RISING_EDGE);
    gpio_enable_event_detection (BUTTON_UP, GPIO_DETECT_RISING_EDGE);
    gpio_enable_event_detection (BUTTON_DOWN, GPIO_DETECT_RISING_EDGE);

    /* enable gpio interrupts */
    gpio_interrupts_enable ();
    interrupts_global_enable ();
}
