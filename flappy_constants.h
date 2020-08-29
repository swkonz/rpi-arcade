#include "gpio.h"

/* screen size */
#define _WIDTH                                 640
#define _HEIGHT                                512

/* color definitions */
#define FB_LIGHT_GREEN      gl_color(155, 230, 88)
#define FB_GREEN            gl_color(115, 191, 46)
#define FB_DARK_GREEN        gl_color(90, 139, 37)
#define FB_BLUE            gl_color(105, 188, 198)
#define FB_YELLOW           gl_color(212, 191, 40)
#define BCKGRND_COLOR                      FB_BLUE

/* Bird constants */
#define BIRD_WIDTH                              15
#define GRAVITY                                  1
#define BIRD_JUMP                                4

/* Pipe constants */
#define PIPE_WIDTH                              32
#define PIPE_HEIGHT                            200
#define PIPE_GAP                               130 // vertical gap between north and south
#define PIPE_TOP_HEIGHT                         10 // height of pipe topper
#define PIPE_BORDER                              1 // width of border
#define PIPE_WIDTH_OFFSET                        4 // how much wider the pipe topper should be
#define PIPE_DX                                  2
#define PIPE_NORTH                               0
#define PIPE_SOUTH                               1

#define FONT_WIDTH                              14 // width
#define FONT_HEIGHT                             16 // height
#define FONT_DEPTH                               4 // depth
#define FONT_SIZE                              896 // size = width * height * depth

/* gameplay */
#define SCORE_Y                                100
