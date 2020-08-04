#include "printf.h"
#include "timer.h"
#include "gl.h"
#include "gl_extra.h"
#include "constants.h"
#include "grid.h"

//pointer to grid
// unsigned int maze = malloc(380);

int grid[20][19] = {
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 2, 2, 2, 2, 2, 2, 2, 0, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 1, 0, 2, 0, 0, 2, 0, 0, 2, 0, 1, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 0, 2, 2, 2, 2, 2, 2, 2, 0, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 0, 2, 2, 2, 2, 2, 2, 2, 0, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0},
    {0, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0},
    {0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

void draw_wall(int x, int y)
{
    gl_draw_rect(x, y, WALL_WIDTH, WALL_HEIGHT, GL_BLUE);
}

int get_row(int y)
{
    return y / WALL_HEIGHT;
}

int get_col(int x)
{
    return x / WALL_WIDTH;
}

void draw_food(int x, int y)
{
    gl_draw_circle(x, y, FOOD_RADIUS, GL_YELLOW);
}

void draw_maze()
{
    unsigned int cur_x = 0;
    unsigned int cur_y = 0;

    for (int i = 0; i < 20; i++)
    {
        cur_x = 0;
        for (int j = 0; j < 19; j++)
        {
            //if 0 -> wall   if 1 -> dot   if 2 -> nothing
            // printf("x : %d  y: %d", cur_x, cur_y);
            if (grid[i][j] == 0)
            {
                draw_wall(cur_x, cur_y);
            }
            else if (grid[i][j] == 1)
            {
                //extra math to put the circle at the center of the block
                draw_food(cur_x + 14, cur_y + 11);
            }
            //now update working block
            cur_x = (cur_x + WALL_WIDTH) % _WIDTH;
        }
        cur_y += WALL_HEIGHT % _HEIGHT;
    }
}
