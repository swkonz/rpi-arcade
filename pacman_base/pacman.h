#ifndef PACMAN_H
#define PACMAN_H
#include "constants.h"

typedef struct {
	int x; 
	int y;
	unsigned curr_direction; 
	unsigned future_direction;
} pacman_t;

pacman_t* p_new_pacman(int x, int y);
void p_reset_pacman(pacman_t* pacman);
void p_update_pacman(pacman_t* pacman);
void p_food_collision(pacman_t* pacman);
void p_ghost_collision(pacman_t* pacman);

#endif