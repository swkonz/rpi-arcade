#include "buttons.h"
#include "constants.h"
#include "debounce.h"
#include "flappy_bird.h"
#include "gl.h"
#include "gpio.h"
#include "gpioextra.h"
#include "gpio_interrupts.h"
#include "interrupts.h"
#include "pacman_game.h"
#include "printf.h"
#include "strings.h"
#include "system.h"
#include "timer.h"
#include "printf.h"

/* Static globals */
static unsigned state = 0;
int score = 0;                  // used for keeping game scores

static bool handler_a (unsigned pc) 
{
    printf("A button\n");
    if (debounce(BUTTON_A) && gpio_check_and_clear_event(BUTTON_A))
    {
        state |= 1 << (BUTTON_A - BUTTON_FIRST);            // set first bit
    } 

    return true;
}

static bool handler_up (unsigned pc) 
{
    printf("up\n");
    if (debounce(BUTTON_UP) && gpio_check_and_clear_event(BUTTON_UP))
    {
        state |= 1 << (BUTTON_UP - BUTTON_FIRST);            // set first bit
    } 

    return true;
}

static bool handler_down (unsigned pc) 
{
    printf("down\n");
    if (debounce(BUTTON_DOWN) && gpio_check_and_clear_event(BUTTON_DOWN))
    {
        state |= 1 << (BUTTON_DOWN - BUTTON_FIRST);            // set first bit
    } 

    return true;
}


static bool handler_left (unsigned pc) 
{
    if (gpio_check_and_clear_event(BUTTON_LEFT))
        printf("left\n");

    return true;
}

static bool handler_right (unsigned pc) 
{
    if (gpio_check_and_clear_event(BUTTON_RIGHT))
        printf("right\n");

    return true;
}

static bool handler_b (unsigned pc) 
{
    if (gpio_check_and_clear_event(BUTTON_B))
        printf("button b\n");

    return true;
}


void initialize()
{   
    /* systems controls */
    system_enable_cache();

    /* Graphics */
    gl_init (WIDTH, HEIGHT, GL_DOUBLEBUFFER);

    /* Setup landing screen interrupts */
    interrupts_setup ();
}


/**
 * get_bit: get the state of a bit in an unsigned int
 */
bool get_bit (unsigned num, unsigned bit)
{
    return (num >> bit) & 0x1;
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

    /* setup interrupts */
    gpio_interrupts_register_handler (BUTTON_A, handler_a);
    gpio_interrupts_register_handler (BUTTON_B, handler_b);
    gpio_interrupts_register_handler (BUTTON_UP, handler_up);
    gpio_interrupts_register_handler (BUTTON_DOWN, handler_down);
    gpio_interrupts_register_handler (BUTTON_LEFT, handler_left);
    gpio_interrupts_register_handler (BUTTON_RIGHT, handler_right);

    static unsigned selected = 0;
    while (1)
    {
        display_start_header();

        /* handle controls inputs */
        if (get_bit (state, BUTTON_UP - BUTTON_FIRST) )
            selected = selected == 0 ? 0 : selected - 1;
        if (get_bit (state, BUTTON_DOWN - BUTTON_FIRST) ) 
            selected = selected == MAX_GAME_ID ? MAX_GAME_ID : selected + 1;
        if (get_bit (state, BUTTON_A - BUTTON_FIRST) ) {
            score = 0;
            // start game
            if (selected == 0)
                flappy_bird ();
            else play_pacman ();
        }

        printf ("selected: %d\n", selected);

        // clear the state
        state = 0;

        // draw rectangle over whichever option is selected
        gl_draw_rect (get_center_position_x (selected == 0 ? FLAPPY : PACMAN),
                     150 + (gl_get_char_height () * 2 * selected),
                     gl_get_char_width () * strlen (selected == 0 ? FLAPPY : PACMAN),
                     gl_get_char_height (),
                     LANDING_BKGD_TEXT_COLOR);

        gl_draw_string (get_center_position_x (FLAPPY),
                       150,
                       FLAPPY,
                       LANDING_TEXT_COLOR);

        gl_draw_string (get_center_position_x (PACMAN),
                       150 + gl_get_char_height () + 20,
                       PACMAN,
                       LANDING_TEXT_COLOR);
        
        // swap buffer
        gl_swap_buffer ();

        timer_delay_ms (50);
    }

}