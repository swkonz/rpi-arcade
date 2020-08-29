#ifndef GHOST_H
#define GHOST_H

typedef struct {
    int x; 
    int y;
    unsigned direction; 
    unsigned col; 
} ghost_t;

extern int grid[20][19];

ghost_t* g_new_ghost(int x, int y, unsigned c, unsigned direction);
void g_update_ghost(ghost_t* ghost);

#endif
