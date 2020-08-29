#include "bird.h"
#include "constants.h"
#include "flappy_constants.h"
#include "gpio.h"
#include "gpioextra.h"
#include "gpio_interrupts.h"
#include "gl.h"
#include "interrupts.h"
#include "malloc.h"
#include "pipe.h"
#include <stdbool.h>
#include "system.h"
#include "timer.h"

#define NUM_PIPES 5

static bird_t* bird;
static pipe_t* pipes[NUM_PIPES];
static volatile int start_game = 0;
extern int score;


/******* Interrupts *******/

/** interrupt_handler
 *  Handle button events for the game
 */
bool interrupt_handler(unsigned pc) {
    if (gpio_check_and_clear_event (BUTTON_A))
    {
        if (!start_game)
            start_game = 1;
        else bird->vy = -BIRD_JUMP;
    }
    return true;
}

// ******************** MAIN ******************** //

// not expecting more than double digit score...
static void draw_score(int score) {
    if (score / 10 == 0) {
        gl_draw_char((WIDTH - FONT_WIDTH) / 2, SCORE_Y, score + '0', GL_WHITE);
    } else {
        gl_draw_char(WIDTH/2 - FONT_WIDTH, SCORE_Y, (score/10) + '0', GL_WHITE);
        gl_draw_char(WIDTH/2, SCORE_Y, (score%10) + '0', GL_WHITE);
    }
}

void flappy_bird () 
{
    /* Initialize Button handler */
    handler_fn_t prev_handler = gpio_interrupts_register_handler (BUTTON_A, interrupt_handler);

    /* Main game ops */
    score = 0;
    int game_over = 0;

    // Draw background
    gl_clear(BCKGRND_COLOR);

    // Create bird
    bird = b_new_bird(30, (HEIGHT - BIRD_WIDTH) / 2, BIRD_WIDTH, BIRD_WIDTH, FB_YELLOW);
    b_draw_bird(bird);

    // Create pipes
    for (int i = 0; i < NUM_PIPES; i++)
        pipes[i] = p_new_pipe(i * ((WIDTH + PIPE_WIDTH) / NUM_PIPES));

    gl_draw_string((WIDTH - (FONT_WIDTH*9)) / 2, (HEIGHT - FONT_HEIGHT) / 2, "GET READY", GL_WHITE);
    gl_swap_buffer();

    // Game loop
    while (!start_game) {} // wait until player is ready
    while (1) {
        int start = timer_get_ticks();

        // Clear screen
        gl_clear(BCKGRND_COLOR);

        // Redraw objects
        b_update_bird(bird);
        for (int i = 0; i < NUM_PIPES; i++)
            p_update_pipe(pipes[i]);
        draw_score(score);

        // Have to check collisions before swapping buffer otherwise lose information
        // Have to break after swapping buffer otherwise looks like collision didn't happen
        game_over = b_check_collisions(bird) || (bird->y + bird->height >= HEIGHT);
        gl_swap_buffer();
        if (game_over) break;

        // Delay to match refresh rate of screen
        timer_delay_us(16667 - (timer_get_ticks() - start)); // 1 million / 60 fps = 16667
    }

    // Draw "game over" label
    gl_draw_string((WIDTH - (FONT_WIDTH*9)) / 2, (HEIGHT - FONT_HEIGHT) / 2, "GAME OVER", GL_WHITE);
    gl_swap_buffer();
    start_game = 0;
    timer_delay(2);

    // Free allocated memory
    free(bird);
    for (int i = 0; i < NUM_PIPES; i++)
        free(pipes[i]);
    
    /* reset handler */
    gpio_interrupts_register_handler (BUTTON_A, prev_handler);
}
