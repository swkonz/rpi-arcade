#ifndef PIPE_H
#define PIPE_H

#include "gl.h"

typedef struct {
    int x;
    int y_offset;
    int north_height;
    int south_height;
} pipe_t;

pipe_t* p_new_pipe(int x_offset);

void p_update_pipe(pipe_t* pipe);

#endif