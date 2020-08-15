#include "gl.h"
#include "constants.h"
#include "strings.h"
#include "interrupts.h"
#include "gpio_interrupts.h"
#include "gpio.h"
#include "gpioextra.h"
#include "printf.h"
#include "timer.h"

#include "debounce.h"

/* Static globals */
static unsigned selected = 0;

/** interrupt_handler
 *  Handle button events for the game
 */
static bool interrupt_handler(unsigned pc)
{   
    // handle button press
    if (debounce(BUTTON_A) && gpio_check_and_clear_event(BUTTON_A))
    {
        selected = (selected + 1) % NUM_GAMES;
    }
    return true;
}

static bool interrupt_handler_b(unsigned pc) 
{
    // handle button press
    if (debounce(BUTTON_A) && gpio_check_and_clear_event(BUTTON_B))
    {
        selected = (selected + 1) % NUM_GAMES;
    }
    return true;
}

void initialize()
{
    /* Graphics */
    gl_init(WIDTH, HEIGHT, GL_DOUBLEBUFFER);

    /* IO */
    gpio_init();
    gpio_set_input(BUTTON_A);
    gpio_set_pullup(BUTTON_A);
    debounce_init(BUTTON_A);
    gpio_set_input(BUTTON_B);
    gpio_set_pullup(BUTTON_B);
    debounce_init(BUTTON_B);
    // gpio_set_input(BUTTON_LEFT);
    // gpio_set_pullup(BUTTON_LEFT);
    // gpio_set_input(BUTTON_RIGHT);
    // gpio_set_pullup(BUTTON_RIGHT);
    // gpio_set_input(BUTTON_UP);
    // gpio_set_pullup(BUTTON_UP);
    // gpio_set_input(BUTTON_DOWN);
    // gpio_set_pullup(BUTTON_DOWN);

    /* Interrupts Init */
    interrupts_init();
    gpio_interrupts_init();

    /* Interrupt Events */
    gpio_enable_event_detection(BUTTON_A, GPIO_DETECT_RISING_EDGE);
    gpio_enable_event_detection(BUTTON_B, GPIO_DETECT_RISING_EDGE);
    // gpio_enable_event_detection(BUTTON_LEFT, GPIO_DETECT_RISING_EDGE);
    // gpio_enable_event_detection(BUTTON_RIGHT, GPIO_DETECT_RISING_EDGE);
    // gpio_enable_event_detection(BUTTON_UP, GPIO_DETECT_RISING_EDGE);
    // gpio_enable_event_detection(BUTTON_DOWN, GPIO_DETECT_RISING_EDGE);

    /* Register Interrupt handlers */
    gpio_interrupts_register_handler(BUTTON_A, interrupt_handler);
    gpio_interrupts_register_handler(BUTTON_B, interrupt_handler_b);

    /* enable gpio interrupts */
    gpio_interrupts_enable();
    interrupts_global_enable();
}

/**
 * get_center_pixel_x: Find pixel for this string to be centered
 *     Finds the center pixel value that will make this string appear centered on the screen
 * 
 * @param str: String to find the offset of. Must be valid, null-terminated c-string
 */
static int get_center_position_x(const char *str)
{
    return (gl_get_width() / 2) - (gl_get_char_width() * strlen(str) / 2);
}

void display_start_header()
{
    // draw background
    gl_clear(LANDING_BKGD_COLOR);

    // header text
    static const char *WELCOME = "Welcome!";
    static const char *SELECT = "Select an option!";
    gl_draw_string(get_center_position_x(WELCOME),
                   100,
                   WELCOME,
                   LANDING_TEXT_COLOR);

    gl_draw_string(get_center_position_x(SELECT),
                   120,
                   SELECT,
                   LANDING_TEXT_COLOR);
}

void main()
{
    initialize();

    static const char *FLAPPY = "Flappy Bird";
    static const char *PACMAN = "Pacman";

    while (1)
    {
        display_start_header();

        // draw rectangle over whichever option is selected
        gl_draw_rect(get_center_position_x(selected == 0 ? FLAPPY : PACMAN),
                     150 + (gl_get_char_height() * 2 * selected),
                     gl_get_char_width() * strlen(selected == 0 ? FLAPPY : PACMAN),
                     gl_get_char_height(),
                     LANDING_BKGD_TEXT_COLOR);

        gl_draw_string(get_center_position_x(FLAPPY),
                       150,
                       FLAPPY,
                       LANDING_TEXT_COLOR);

        gl_draw_string(get_center_position_x(PACMAN),
                       150 + gl_get_char_height() + 20,
                       PACMAN,
                       LANDING_TEXT_COLOR);

        // swap buffer
        gl_swap_buffer();

        timer_delay_ms(50);
    }

}