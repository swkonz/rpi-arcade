#include "constants.h"
#include "printf.h"
#include "timer.h"
#include "gpio.h"
#include "gpioextra.h"
#include "gpio_interrupts.h"
#include "interrupts.h"
#include "uart.h"
#include "pacman_constants.h"
#include "grid.h"
#include "gl.h"
#include "system.h"
#include "pacman.h"
#include "ghost.h"
#include "timer.h"

static pacman_t *pacman;
static ghost_t *ghost[4];

int score;
int lives;
int died;
int counter = 0;

// ***************** INTERRUPTS ***************** //

static bool handler_up (unsigned pc)
{
    if (gpio_check_and_clear_event(BUTTON_UP))
    {
        pacman->future_direction = NORTH;
    }
    return true;
}

static bool handler_down (unsigned pc)
{
    if (gpio_check_and_clear_event(BUTTON_DOWN))
    {
        pacman->future_direction = SOUTH;
    }
    return true;
}

static bool handler_right (unsigned pc)
{
    if (gpio_check_and_clear_event(BUTTON_RIGHT))
    {
        pacman->future_direction = EAST;
    }
    return true;
}

static bool handler_left (unsigned pc)
{
    if (gpio_check_and_clear_event(BUTTON_LEFT))
    {
        pacman->future_direction = WEST;
    }
    return true;
}

// not expecting more than a three digit scores
static void draw_score_and_lives()
{
    gl_draw_string(SCORE_X, SCORE_Y, "score:", GL_WHITE);
    if (score / 10 == 0)
    {
        gl_draw_char(10 + FONT_WIDTH * 7, SCORE_Y, score + '0', GL_WHITE);
    }
    else if (score / 100 == 0)
    {
        gl_draw_char(SCORE_X + FONT_WIDTH * 7, SCORE_Y, (score / 10) + '0', GL_WHITE);
        gl_draw_char(SCORE_X + FONT_WIDTH * 8 + 1, SCORE_Y, (score % 10) + '0', GL_WHITE);
    }
    else
    {
        gl_draw_char(SCORE_X + FONT_WIDTH * 7, SCORE_Y, (score / 100) + '0', GL_WHITE);
        gl_draw_char(SCORE_X + FONT_WIDTH * 8 + 1, SCORE_Y, (score / 10 % 10) + '0', GL_WHITE);
        gl_draw_char(SCORE_X + FONT_WIDTH * 9 + 2, SCORE_Y, (score % 100 % 10) + '0', GL_WHITE);
    }

    gl_draw_string(LIVES_X, SCORE_Y, "lives:", GL_WHITE);
    for (int i = 0; i < lives; i++)
        gl_draw_char(LIVES_X + FONT_WIDTH * (7 + i) + (1 + i), SCORE_Y, '*', GL_WHITE);
}

// ******************** MAIN ******************** //

// Will's super cool timing macro
#define TIMER_BLOCK(NAME, BLOCK)                               \
    {                                                          \
        int time = timer_get_time();                           \
        BLOCK;                                                 \
        printf("%s: %dus\n", (NAME), timer_get_time() - time); \
    }

void play_pacman()
{
    /* switch interrupt handlers */
    handler_fn_t prev_up = gpio_interrupts_register_handler (BUTTON_UP, handler_up);
    handler_fn_t prev_down = gpio_interrupts_register_handler (BUTTON_DOWN, handler_down);
    handler_fn_t prev_left = gpio_interrupts_register_handler (BUTTON_LEFT, handler_left);
    handler_fn_t prev_right = gpio_interrupts_register_handler (BUTTON_RIGHT, handler_right);

    score = 0;
    lives = 3;
    died = 0;

    gl_clear(BCKGRND_COLOR);
    pacman = p_new_pacman(PACMAN_START_X, PACMAN_START_Y);

    ghost[0] = g_new_ghost(GHOST_START_X, GHOST_START_Y, GHOST_BLINKY, NORTH);
    ghost[1] = g_new_ghost(GHOST_START_X, GHOST_START_Y, GHOST_INKY, SOUTH);
    ghost[2] = g_new_ghost(GHOST_START_X, GHOST_START_Y, GHOST_PINKY, EAST);
    ghost[3] = g_new_ghost(GHOST_START_X, GHOST_START_Y, GHOST_CLYDE, WEST);

    while (1)
    {
        int start = timer_get_ticks();

        gl_clear(BCKGRND_COLOR);
        draw_maze();

        // Draw ghosts
        for (int i = 0; i < NUM_GHOSTS; i++)
        {
            g_update_ghost(ghost[i]);
        }

        // Draw pacman
        p_update_pacman(pacman);

        // Check collisions
        p_food_collision(pacman);
        p_ghost_collision(pacman);

        draw_score_and_lives();

        gl_swap_buffer();

        if (died)
        {
            lives--;
            died = 0;
            draw_score_and_lives();
            gl_swap_buffer();
            if (lives == 0)
                break; // lost
            timer_delay(3);
            p_reset_pacman(pacman);
        }
        else if (score == NUM_FOODS)
        {
            break; // won!
        }
        else
        {
            // Delay to match refresh rate of screen
            timer_delay_us(33333 - (timer_get_ticks() - start)); // 1 million / 60 fps = 16667
        }
    }

    if (died && lives == 0)
    {
        gl_draw_string((WIDTH - (FONT_WIDTH * 9)) / 2, (HEIGHT - FONT_HEIGHT) / 2, "GAME OVER", GL_WHITE);
    }
    else
    {
        gl_draw_string((WIDTH - (FONT_WIDTH * 8)) / 2, (HEIGHT - FONT_HEIGHT) / 2, "YOU WON!", GL_WHITE);
    }
    gl_swap_buffer();
    timer_delay (3);

    /* swap back to original interrupt handlers */
    gpio_interrupts_register_handler (BUTTON_UP, prev_up);
    gpio_interrupts_register_handler (BUTTON_DOWN, prev_down);
    gpio_interrupts_register_handler (BUTTON_LEFT, prev_left);
    gpio_interrupts_register_handler (BUTTON_RIGHT, prev_right);
}