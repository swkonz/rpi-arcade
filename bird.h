#ifndef BIRD_H
#define BIRD_H

#include "constants.h"
#include "flappy_constants.h"


typedef struct {
    int x;
    int y;
    int vy;
    unsigned width;
    unsigned height;
    color c;
    int move_count;
} bird_t;

bird_t* b_new_bird(int x, int y, int width, int height, color c);

void b_draw_bird(bird_t* bird);
void b_clear_bird(bird_t* bird);
void b_update_bird(bird_t* bird);
int b_check_collisions(bird_t* bird);

#endif
