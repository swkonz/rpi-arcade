#include "pipe.h"
#include "flappy_constants.h"
#include "gl.h"
#include "gl_extra.h"
#include "malloc.h"
#include "printf.h"
#include "rand.h"

extern int score;

// ******************** RANDOM HELPERS ******************** //
static unsigned get_random_offset() {
	unsigned r = rand();
	if (r % 2 == 0) {
		return (r % 10) * 8;
	} else {
		return -(r % 10) * 8;
	}
}
// ******************** DRAWING HELPERS ******************** //
/** 
 * Actually draw the pipe on the screen. Handles drawing only a single pipe
 */
static void draw_pipe(int location, int x, int height) {
    // Draw long body
    int y = (location == PIPE_NORTH) ? 0 : _HEIGHT - height;

    gl_draw_rect(x, y, PIPE_WIDTH / 4, height, FB_LIGHT_GREEN);
    gl_draw_rect(x + PIPE_WIDTH / 4, y, PIPE_WIDTH / 2, height, FB_GREEN);
    gl_draw_rect(x + PIPE_WIDTH * 3 / 4, y, PIPE_WIDTH / 4, height, FB_DARK_GREEN);
    gl_add_border_to(x, y, PIPE_WIDTH, height, PIPE_BORDER, GL_BLACK);

    // Draw pipe topper
    int top_height = PIPE_TOP_HEIGHT;
    int top_width = PIPE_WIDTH + PIPE_WIDTH_OFFSET;
    int top_x = x - PIPE_WIDTH_OFFSET / 2;
    int top_y = (location == PIPE_NORTH) ? (y + height + PIPE_BORDER) : (y - top_height - PIPE_BORDER);

    gl_draw_rect(top_x, top_y, top_width / 4, top_height, FB_LIGHT_GREEN);
    gl_draw_rect(top_x + top_width / 4, top_y, top_width / 2, top_height, FB_GREEN);
    gl_draw_rect(top_x + top_width * 3 / 4, top_y, top_width / 4, top_height, FB_DARK_GREEN);
    gl_add_border_to(top_x, top_y, top_width, top_height, 1, GL_BLACK);
}

/**
 * Draw a pair of pipes on the screen. One protruding down, one up
 */
static void draw_pipe_pair(int x, int north_height, int south_height) {
	draw_pipe(PIPE_NORTH, x, north_height);
	draw_pipe(PIPE_SOUTH, x, south_height);
}

/**
 * Determine the height the pipe protrudes from it's start point
 */
static void calculate_heights(pipe_t* pipe) {
	pipe->north_height = (_HEIGHT - PIPE_TOP_HEIGHT*2 - PIPE_GAP) / 2 + pipe->y_offset;
	pipe->south_height = (_HEIGHT - PIPE_TOP_HEIGHT*2 - PIPE_GAP) / 2 - pipe->y_offset;
}

// ******************** PUBLIC INTERFACE ******************** //
/**
 * Create a new pipe object, used to track pipe locations on the screen
 */
pipe_t* p_new_pipe(int x_offset) {
	pipe_t* pipe = malloc(sizeof(pipe_t));
	pipe->x = x_offset;
	pipe->y_offset = get_random_offset();
	calculate_heights(pipe);
	return pipe;
}

/**
 * Redraw the pipe in a new location on the screen
 */
void p_update_pipe(pipe_t* pipe) {
	// Wrap around
	if (pipe->x <= -PIPE_WIDTH) {
		pipe->x = _WIDTH;
		pipe->y_offset = get_random_offset();
		calculate_heights(pipe);
		score++;
	}

	pipe->x -= PIPE_DX;
	draw_pipe_pair(pipe->x, pipe->north_height, pipe->south_height);
}