#include "gl.h"
#include "printf.h"
#include "font.h"
#include "constants.h"

#define BYTES_PER_PIXEL (fb_get_depth() / 8)

static int min(int a, int b) {
    return (a < b) ? a : b;
}

static int max(int a, int b) {
    return (a > b) ? a : b;
}

// =========== BASIC GRAPHICS =========== //

void gl_init(unsigned width, unsigned height, unsigned mode) {
    fb_init(width, height, 32 / 8, mode);
}

void gl_swap_buffer() {
    fb_swap_buffer();
}

color gl_color(unsigned char r, unsigned char g, unsigned char b) {
    return (r << 16) | (g << 8) | b;
}

/*
 * Clears every pixel on the screen and sets it to the provided color.
 */
void gl_clear(color c) {
    unsigned long long* pixel = (unsigned long long*) fb_get_draw_buffer();
    int num_pixels = (fb_get_pitch() / BYTES_PER_PIXEL) * fb_get_height();

    unsigned long long u = c;
    unsigned long long uu = (u << 32) | u;

    for (int i = 0; i < num_pixels / 2; i += 8) {
        unsigned long long* fbll = (unsigned long long*) pixel;
        fbll[0] = uu;
        fbll[1] = uu;
        fbll[2] = uu;
        fbll[3] = uu;
        fbll[4] = uu;
        fbll[5] = uu;
        fbll[6] = uu;
        fbll[7] = uu;
        pixel += 8;
    }
}

/* 
 * Returns a pointer to the pixel at the requested coordinate. Assumes x and y are in bounds.
 */
static color* get_pixel(int x, int y) {
    // int pitch_bytes = fb_get_pitch(); // hardcoded as 2560
    // int pitch_pixels = 2560 / BYTES_PER_PIXEL;

    color (*pixels)[_WIDTH] = (color (*)[_WIDTH]) fb_get_draw_buffer();
    // Scale x coordinate to fit a potentially larger pitch
    // int adjusted_x = (int) ( ((float)x / fb_get_width()) * pitch_pixels );
    return &(pixels[y][x]);
}

void gl_draw_pixel(int x, int y, color c) {
    // Bounds checking
    // if (y < 0 || y >= fb_get_height()) return;
    // if (x < 0 || x >= fb_get_width()) return;

    *get_pixel(x, y) = c;
}

color gl_read_pixel(int x, int y) {
    // Bounds checking
    // if (y < 0 || y >= fb_get_height()) return 0;
    // if (x < 0 || x >= fb_get_width()) return 0;

    return *get_pixel(x, y);
}

void gl_draw_rect(int x, int y, int w, int h, color c) {
    if (x < 0) {
        w = max(0, w+x); x = 0;
    } else if (x+w >= _WIDTH) {
        w = w - max(0, min(w, x+w-_WIDTH));
    }

    color (*pixels)[_WIDTH] = (color (*)[_WIDTH]) fb_get_draw_buffer();

    for (int dx = 0; dx < w; dx++) {
        for (int dy = 0; dy < h; dy++) {
            pixels[y+dy][x+dx] = c;
        }
    }
}

// Draws an one-pixel wide inner border (of color bc) for a rectangle of the specified
// size.
void gl_draw_inner_border_rect(int x, int y, int w, int h, color c, color bc) {
    if (x < 0) {
        w = max(0, w+x); x = 0;
    } else if (x+w >= _WIDTH) {
        w = w - max(0, min(w, x+w-_WIDTH));
    }

    color (*pixels)[_WIDTH] = (color (*)[_WIDTH]) fb_get_draw_buffer();

    for (int dy = 0; dy < h; dy++) {
        pixels[y+dy][x] = bc;
    }
    for (int dx = 1; dx < w-1; dx++) {
        pixels[y][x+dx] = bc;
        for (int dy = 1; dy < h-1; dy++) {
            pixels[y+dy][x+dx] = c;
        }
        pixels[y+h-1][x+dx] = bc;
    }
    for (int dy = 0; dy < h; dy++) {
        pixels[y+dy][x+w-1] = bc;
    }
}

// Adds border of specified color and width to a rectangular area.
void gl_add_border_to(int x, int y, int width, int height, int border_width, color border_c) {
    gl_draw_rect(x-border_width, y, border_width, height+border_width, border_c); // left border
    gl_draw_rect(x+width, y, border_width, height+border_width, border_c); // right border
    gl_draw_rect(x-border_width, y-border_width, width+border_width*2, border_width, border_c); // top border
    gl_draw_rect(x, y+height, width, border_width, border_c); // bottom border
}

// x and y are in the top left corner
// https://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles
void gl_draw_circle(int x, int y, int r, color c) {
    color (*pixels)[_WIDTH] = (color (*)[_WIDTH]) fb_get_draw_buffer();

    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx++) {
            if (dx*dx + dy*dy <= r*r+r/3)
                pixels[y+r+dy][x+r+dx] = c;
        }
    }
}

/** ------------------- LINE / TRIANGLE DRAWING -------------------- **/

// Variant of Xu's algorithm
// Explanation used from: https://unionassets.com/blog/algorithm-brezenhema-and-wu-s-line-299
void gl_draw_line(int x0, int y0, int x1, int y1, color c) {

}

void gl_draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color c) {
    
}

// =========== DISPLAY TEXT =========== //

void gl_draw_char(int x, int y, char ch, color c) {
    // Bounds checking: if part of character will be off the screen, don't draw
    // if (x < 0 || x + FONT_WIDTH >= fb_get_width()) return;
    // if (y < 0 || y + FONT_HEIGHT >= fb_get_height()) return;

    // Get array of pixels for the given character
    char buf[FONT_SIZE];
    int success = font_get_char(ch, buf, FONT_SIZE);
    if (!success) return;

    // Draw each pixel in the array either as on or off
    for (int dy = 0; dy < FONT_HEIGHT; dy++) {
        for (int dx = 0; dx < FONT_WIDTH; dx++) {
            int pixel = buf[dx * FONT_DEPTH + dy * (FONT_WIDTH * FONT_DEPTH)];
            if (pixel) gl_draw_pixel(x + dx, y + dy, c);
            // else       gl_draw_pixel(x + dx, y + dy, 0);
        }
    }

}

void gl_draw_string(int x, int y, char* str, color c) {
    int char_index = 0;

    while (*str != '\0') {
        gl_draw_char(x + (char_index * FONT_WIDTH), y, *str, c);
        str++;
        char_index++;
    }
}
