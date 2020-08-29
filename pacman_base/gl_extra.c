#include "pacman_constants.h"
#include "gl_extra.h"

// x and y are in the top left corner
// https://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles
void gl_draw_circle(int x, int y, int r, color c)
{
    color(*pixels)[_WIDTH] = (color(*)[_WIDTH])fb_get_draw_buffer();

    for (int dy = -r; dy <= r; dy++)
    {
        for (int dx = -r; dx <= r; dx++)
        {
            if (dx * dx + dy * dy <= r * r + r / 3)
                pixels[y + r + dy][x + r + dx] = c;
        }
    }
}