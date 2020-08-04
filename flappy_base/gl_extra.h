#ifndef GL_EXTRA_H
#define GL_EXTRA_H

#include "gl.h"

void gl_draw_inner_border_rect(int x1, int y1, int width, int height, color_t c, color_t bc);
void gl_add_border_to(int x, int y, int width, int height, int border_width, color_t border_c);
void gl_draw_circle(int x, int y, int r, color_t c);

#endif