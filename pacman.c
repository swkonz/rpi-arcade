#include "pacman.h"
#include "pacman_constants.h"
#include "constants.h"
#include "gl.h"
#include "fb.h"
#include "malloc.h"
#include "printf.h"
#include "grid.h"

extern int grid[20][19];
extern int score;
extern int died;
extern color ghost_colors[4];

static void draw_pacman(int top_left_x, int top_left_y, int r, int direction) {
    color (*pixels)[WIDTH] = (color (*)[WIDTH]) fb_get_draw_buffer();

    int x = top_left_x + WIDTH / 38;
    int y = top_left_y + HEIGHT / 40;

    // Draw yellow body
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx++) {
            if (dx*dx + dy*dy <= r*r+r/3)
                pixels[y+dy][x+dx] = PACMAN_YELLOW;
        }
    }

    // Draw black mouth over
    switch(direction) {
    case NORTH:
        for (int dy = -r; dy < 0; dy++) {
            for (int dx = dy/2; dx <= -dy/2; dx++) {
                pixels[y+dy][x+dx] = BCKGRND_COLOR;
            }
        }
        break;
    case WEST:
        for (int dx = -r; dx < 0; dx++) {
            for (int dy = dx/2; dy <= -dx/2; dy++) {
                pixels[y+dy][x+dx] = BCKGRND_COLOR;
            }
        }
        break;
    case SOUTH:
        for (int dy = 1; dy <= r; dy++) {
            for (int dx = -dy/2; dx <= dy/2; dx++) {
                pixels[y+dy][x+dx] = BCKGRND_COLOR;
            }
        }
        break;
    case EAST:
        for (int dx = 1; dx <= r; dx++) {
            for (int dy = -dx/2; dy <= dx/2; dy++) {
                pixels[y+dy][x+dx] = BCKGRND_COLOR;
            }
        }
        break;
    }
}

void update_direction(pacman_t* pacman) {
    int center_x = pacman->x + WALL_HALF_WIDTH;
    int center_y = pacman->y + WALL_HALF_HEIGHT;
    int right_x = pacman->x + WALL_WIDTH - 1;
    int bottom_y = pacman->y + WALL_HEIGHT - 1;

    if (pacman->curr_direction != pacman->future_direction) {
        switch(pacman->future_direction) {
        case NORTH:
            if (pacman->y > PACMAN_NORTH_LIMIT && grid[get_row(bottom_y) - 1][get_col(center_x)] != WALL &&
                ((pacman->x % WALL_WIDTH) <= 1 || (pacman->x % WALL_WIDTH >= 32)))
                pacman->curr_direction = pacman->future_direction;
            break;
        case SOUTH:
            if (pacman->y < PACMAN_SOUTH_LIMIT && grid[get_row(pacman->y) + 1][get_col(center_x)] != WALL &&
                ((pacman->x % WALL_WIDTH) <= 1 || (pacman->x % WALL_WIDTH >= 32)))
                pacman->curr_direction = pacman->future_direction;
            break;
        case EAST:
            if (pacman->x < PACMAN_EAST_LIMIT && grid[get_row(center_y)][get_col(pacman->x) + 1] != WALL &&
                ((pacman->y % WALL_HEIGHT) <= 1 || (pacman->y % WALL_HEIGHT) >= 24))
                pacman->curr_direction = pacman->future_direction;
            break;
        case WEST:
            if (pacman->x > PACMAN_WEST_LIMIT && grid[get_row(center_y)][get_col(right_x) - 1] != WALL &&
                ((pacman->y % WALL_HEIGHT) <= 1 || (pacman->y % WALL_HEIGHT) >= 24))
                pacman->curr_direction = pacman->future_direction;
            break;
        }
    }
}

// ******************** PUBLIC INTERFACE ******************** //

pacman_t* p_new_pacman(int x, int y) {
	pacman_t* pacman = malloc(sizeof(pacman_t));
	pacman->x = x;
	pacman->y = y;
	pacman->curr_direction = EAST;
    pacman->future_direction = EAST;

    draw_pacman(pacman->x, pacman->y, PACMAN_RADIUS, pacman->curr_direction);

	return pacman;
}

void p_reset_pacman(pacman_t* pacman) {
    pacman->x = PACMAN_START_X;
    pacman->y = PACMAN_START_Y;
    pacman->curr_direction = EAST;
    pacman->future_direction = EAST;
}

void p_update_pacman(pacman_t* pacman) {
	// Bounds check
    update_direction(pacman);

    int center_x = pacman->x + WALL_HALF_WIDTH;
    int center_y = pacman->y + WALL_HALF_HEIGHT;
    int right_x = pacman->x + WALL_WIDTH - 1;
    int bottom_y = pacman->y + WALL_HEIGHT - 1;

	switch(pacman->curr_direction) {
	case NORTH:
        if (pacman->y > PACMAN_NORTH_LIMIT && grid[get_row(bottom_y) - 1][get_col(center_x)] != WALL)
			pacman->y -= PACMAN_DY;
		break;
	case SOUTH:
        if (pacman->y < PACMAN_SOUTH_LIMIT && grid[get_row(pacman->y) + 1][get_col(center_x)] != WALL)
			pacman->y += PACMAN_DY;
		break;
	case EAST:
        if (pacman->x < PACMAN_EAST_LIMIT && grid[get_row(center_y)][get_col(pacman->x) + 1] != WALL)
			pacman->x += PACMAN_DX;
		break;
	case WEST:
        if (pacman->x > PACMAN_WEST_LIMIT && grid[get_row(center_y)][get_col(right_x) - 1] != WALL)
			pacman->x -= PACMAN_DX;
		break;
	}

    draw_pacman(pacman->x, pacman->y, PACMAN_RADIUS, pacman->curr_direction);
}

void p_food_collision(pacman_t* pacman) {
    int row = get_row(pacman->y + WALL_HALF_HEIGHT);
    int col = get_col(pacman->x + WALL_HALF_WIDTH);

    if (grid[row][col] == FOOD) {
        grid[row][col] = EMPTY;
        score++;
    }
}

void p_ghost_collision(pacman_t* pacman) {
    int center_x = pacman->x + WALL_HALF_WIDTH;
    int center_y = pacman->y + WALL_HALF_WIDTH;

    int left = center_x - PACMAN_RADIUS;
    int right = center_x + PACMAN_RADIUS;
    int top = center_y - PACMAN_RADIUS;
    int bottom = center_y + PACMAN_RADIUS;

    for (int i = 0; i < 4; i++) {
        if (gl_read_pixel(left,center_y) == ghost_colors[i] ||
            gl_read_pixel(right,center_y) == ghost_colors[i] ||
            gl_read_pixel(center_x,top) == ghost_colors[i] ||
            gl_read_pixel(center_x,bottom) == ghost_colors[i])
            died = 1;
    }
}
