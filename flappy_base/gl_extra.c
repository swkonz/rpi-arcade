#include "flappy_constants.h"
#include "gl_extra.h"

/* Helper macros */
#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(x, y) (((x) > (y)) ? (x) : (y))

// Draws an one-pixel wide inner border (of color bc) for a rectangle of the specified
// size.
void gl_draw_inner_border_rect(int x, int y, int w, int h, color_t c, color_t bc)
{
    if (x < 0)
    {
        w = max(0, w + x);
        x = 0;
    }
    else if (x + w >= _WIDTH)
    {
        w = w - max(0, min(w, x + w - _WIDTH));
    }

    color_t(*pixels)[_WIDTH] = (color_t(*)[_WIDTH])fb_get_draw_buffer();

    for (int dy = 0; dy < h; dy++)
    {
        pixels[y + dy][x] = bc;
    }
    for (int dx = 1; dx < w - 1; dx++)
    {
        pixels[y][x + dx] = bc;
        for (int dy = 1; dy < h - 1; dy++)
        {
            pixels[y + dy][x + dx] = c;
        }
        pixels[y + h - 1][x + dx] = bc;
    }
    for (int dy = 0; dy < h; dy++)
    {
        pixels[y + dy][x + w - 1] = bc;
    }
}

// Adds border of specified color and width to a rectangular area.
void gl_add_border_to(int x, int y, int width, int height, int border_width, color_t border_c)
{
    gl_draw_rect(x - border_width, y, border_width, height + border_width, border_c);                   // left border
    gl_draw_rect(x + width, y, border_width, height + border_width, border_c);                          // right border
    gl_draw_rect(x - border_width, y - border_width, width + border_width * 2, border_width, border_c); // top border
    gl_draw_rect(x, y + height, width, border_width, border_c);                                         // bottom border
}