#include "bird.h"
#include "gl.h"
#include "gl_extra.h"
#include "malloc.h"

// Initializes and returns a new bird instance.
bird_t* b_new_bird(int x, int y, int width, int height, color c) {
    bird_t* bird = malloc(sizeof(bird_t));
    bird->x = x;
    bird->y = y;
    bird->width = width;
    bird->height = height;
    bird->c = c;
    bird->move_count = 0;
    bird->vy = 1;

    return bird;
}

// Returns true (1) if any corner of the bird has collided with a non-background-
// colored object.
int b_check_collisions(bird_t* bird) {
	return (gl_read_pixel(bird->x - 1, bird->y - 1) != BCKGRND_COLOR ||
		    gl_read_pixel(bird->x + bird->width, bird->y - 1) != BCKGRND_COLOR ||
		    gl_read_pixel(bird->x - 1, bird->y + bird->height) != BCKGRND_COLOR ||
		    gl_read_pixel(bird->x + bird->width, bird->y + bird->height) != BCKGRND_COLOR);
}

// Draw the orange bird square at its current location.
void b_draw_bird(bird_t* bird) {
    gl_draw_inner_border_rect(bird->x, bird->y, bird->width, bird->height, bird->c, GL_BLACK);
}

// TODO: make this interface more modular (esp count)
// Updates the y position and velocity of the bird, then redraws on canvas.
void b_update_bird(bird_t* bird) {
    // Update velocity
    if (bird->move_count == 5) { // only update velocity every __ iterations
        bird->vy += GRAVITY;
        bird->move_count = 0;
    }
    bird->y += bird->vy; // always update y position
    bird->move_count++;

    // Draw new bird
    b_draw_bird(bird);
}