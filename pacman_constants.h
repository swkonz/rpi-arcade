#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "gpio.h"
#include "constants.h"

typedef unsigned color;

#define BACKGROUND_COLOR                  GL_BLACK

#define FONT_WIDTH                              14 // width
#define FONT_HEIGHT                             16 // height
#define FONT_DEPTH                               4 // depth
#define FONT_SIZE                              896 // size = width * height * depth

#define BCKGRND_COLOR                     GL_BLACK
#define PACMAN_YELLOW                   0xFFFFEE00
#define GHOST_BLINKY                    0xffe12000
#define GHOST_INKY                      0xff74fdf8
#define GHOST_PINKY                     0xfffba4a7
#define GHOST_CLYDE                     0xfffea414
#define MAZE_BLUE                       0xff053fe0
#define FOOD_YELLOW                     0xfff8b392

#define PACMAN_RADIUS                            8
#define PACMAN_DX                                2
#define PACMAN_DY                                2

#define PACMAN_START_X                           (WALL_WIDTH * 9)
#define PACMAN_START_Y                           (WALL_HEIGHT * 14)

#define GHOST_START_X                            PACMAN_START_X
#define GHOST_START_Y                            (WALL_HEIGHT * 10)

#define PACMAN_WEST_LIMIT                        WIDTH / 19
#define PACMAN_NORTH_LIMIT                       HEIGHT / 20 * 2
#define PACMAN_EAST_LIMIT                        WIDTH / 19 * 17
#define PACMAN_SOUTH_LIMIT                       HEIGHT - (HEIGHT / 20)

#define NORTH                                    0
#define EAST                                     1
#define SOUTH                                    2
#define WEST                                     3

#define FOOD_RADIUS                              2

#define WALL                                     0
#define FOOD                                     1
#define EMPTY                                    2
#define WALL_WIDTH                    (WIDTH / 19)
#define WALL_HEIGHT                  (HEIGHT / 20)
#define WALL_HALF_WIDTH               (WIDTH / 38)
#define WALL_HALF_HEIGHT             (HEIGHT / 38)

#define NUM_GHOSTS                               4
#define NUM_FOODS                              150

#define SCORE_X                                 10
#define SCORE_Y                                  7
#define LIVES_X             WIDTH - FONT_WIDTH*10



#define SIMILARITY_TOLERANCE                     5
#define NUM_CHUNKS                             300

#define LEXICON_SIZE 2
#define MAX_AUDIO_FILE_CNT 1
#define MAX_FILENAME_LENGTH 20

#endif