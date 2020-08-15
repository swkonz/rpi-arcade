#include "gpio.h"
#include "gl.h"

/* helper typedefs */
typedef unsigned color;

/* helper macros */
#define abs(x)                  (((x) < 0) ? -(x) : (x))

/* Operational Constants */
#define NUM_GAMES               (2)

/* Button definitions */
#define BUTTON_A                GPIO_PIN20
#define BUTTON_B                GPIO_PIN21
#define BUTTON_LEFT             GPIO_PIN22
#define BUTTON_RIGHT            GPIO_PIN23
#define BUTTON_UP               GPIO_PIN24
#define BUTTON_DOWN             GPIO_PIN25

/* Graphics definitions */
#define HEIGHT                  (480)
#define WIDTH                   (620)

/* Landing Screen constants */
#define LANDING_BKGD_COLOR      GL_AMBER
#define LANDING_TEXT_COLOR      GL_PURPLE
#define LANDING_BKGD_TEXT_COLOR GL_CYAN
