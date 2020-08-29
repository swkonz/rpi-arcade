#include "pacman_constants.h"
#include "constants.h"
#include "gl.h"
#include "fb.h"
#include "malloc.h"
#include "printf.h"
#include "rand.h"
#include "grid.h"
#include "ghost.h"

color ghost_colors[4] = {GHOST_BLINKY, GHOST_INKY, GHOST_PINKY, GHOST_CLYDE};

static const int r = 8; 
static void draw_ghost(int top_left_x, int top_left_y, color c) {
    color (*pixels)[WIDTH] = (color (*)[WIDTH]) fb_get_draw_buffer();

    int x = top_left_x + WIDTH / 38;
    int y = top_left_y + HEIGHT / 40;

    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx++) {
            if (dx*dx + dy*dy <= r*r+r/3)
                pixels[y+dy][x+dx] = c;
        }
    }
}


static int is_intersection(ghost_t *ghost){
    int ghost_r = get_row(ghost->y); 
    int ghost_c = get_col(ghost->x); 

    //keep track of num intersections
    int num_intersections = 0;

    // switch(ghost->direction) {

    // }

    if(grid[ghost_r - 1][ghost_c] > 0) {        //north clear
        num_intersections++;
    }
    if(grid[ghost_r + 1][ghost_c] > 0) {        //south clear
        num_intersections++;
    }
    if(grid[ghost_r][ghost_c + 1] > 0) {        //east clear
        num_intersections++;
    }
    if(grid[ghost_r][ghost_c - 1] > 0) {        //west clear
        num_intersections++;
    }
    return num_intersections;
}

static int frontIsClear(ghost_t *ghost) {
    // int ghost_r = get_row(ghost->y); 
    // int ghost_c = get_col(ghost->x); 

    int center_x = ghost->x + WALL_HALF_WIDTH;
    int center_y = ghost->y + WALL_HALF_HEIGHT;
    int right_x = ghost->x + WALL_WIDTH - 1;
    int bottom_y = ghost->y + WALL_HEIGHT - 1;

    switch(ghost->direction) {
    case NORTH:
        if (ghost->y > PACMAN_NORTH_LIMIT && grid[get_row(bottom_y) - 1][get_col(center_x)] != WALL)
            return 1; 
        break;
    case SOUTH:
        if (ghost->y < PACMAN_SOUTH_LIMIT && grid[get_row(ghost->y) + 1][get_col(center_x)] != WALL)
            return 1; 
        break;
    case EAST:
        if (ghost->x < PACMAN_EAST_LIMIT && grid[get_row(center_y)][get_col(ghost->x) + 1] != WALL)
            return 1; 
        break;
    case WEST:
        if (ghost->x > PACMAN_WEST_LIMIT && grid[get_row(center_y)][get_col(right_x) - 1] != WALL)
            return 1; 
        break;
    }  

    return 0; 

}

ghost_t* g_new_ghost(int x, int y, unsigned c, unsigned direction){
    ghost_t* ghost = malloc(sizeof(ghost_t));
    ghost->x = x;
    ghost->y = y;
    ghost->direction = direction; 
    ghost->col = c;
    return ghost;
}

void g_update_ghost(ghost_t* ghost) {
    // Bounds check
    // int direction = rand() % 4; 

    // while((frontIsClear(ghost) == 1) && (is_intersection(ghost) > 2)) {
    //     ghost->direction = rand() % 4; 
    // }
    // while((frontIsClear(ghost) == 0) && (is_intersection(ghost) == 2)) {
    //     ghost->direction = rand() % 4; 
    // }
    // while((frontIsClear(ghost) == 0) && (is_intersection(ghost) == 3)) {
    //     ghost->direction = rand() % 4; 
    // }
    int cur_dir = ghost->direction;
    int intersections = is_intersection(ghost);
    int front_clear = frontIsClear(ghost);
    if((intersections == 2) && !front_clear) {
        // while(ghost->direction == cur_dir || frontIsClear(ghost) == 0) {
        //     ghost->direction = rand() % 4; 
        // }
        ghost->direction = (cur_dir + 5) % 4;
        if(!front_clear){
            ghost->direction = (cur_dir + 7) % 4;
        }
    } 
    // else if((intersections == 3) && (front_clear == 0)) {
    //     switch(rand() % 2) {
    //         case 0:
    //             ghost->direction = (cur_dir + 5) % 4;
    //         case 1:
    //             ghost->direction = (cur_dir + 7) % 4;
    //     }
    // } else if((intersections == 3) && (front_clear == 1)) {
    //     switch(rand() % 2) {
    //         case 0:
    //             ghost->direction = (cur_dir + 5) % 4;
    //             if(frontIsClear(ghost) == 0){
    //                 ghost->direction = (cur_dir + 7) % 4;
    //             }
    //         case 1:
    //             ghost->direction = cur_dir;
    //     }
    // } else if(intersections == 4) {
    //     switch(rand() % 3) {
    //         case 0:
    //             ghost->direction = (cur_dir + 5) % 4;
    //         case 1:
    //             ghost->direction = (cur_dir + 7) % 4;
    //         case 2:
    //             ghost->direction = cur_dir;
    //     }
    // }
    else {
        while((ghost->direction % 2 == cur_dir % 2 &&
         ghost->direction != cur_dir) || !frontIsClear(ghost)) {
            ghost->direction = rand() % 4; 
        }
    }
    while(!frontIsClear(ghost)) {
        ghost->direction = rand() % 4;
    }

    switch(ghost->direction) {
    case NORTH:
        ghost->y -= PACMAN_DY;
        break;
    case SOUTH:
        ghost->y += PACMAN_DY;
        break;
    case EAST:
        ghost->x += PACMAN_DX;
        break;
    case WEST:
        ghost->x -= PACMAN_DX;
        break;
    }
    draw_ghost(ghost->x, ghost->y, ghost->col);
}

