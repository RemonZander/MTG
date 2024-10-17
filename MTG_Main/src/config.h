#ifndef CONFIG_H
#define CONFIG_H

#define BOARD_SIZE_X_MAX 255
#define BOARD_SIZE_Y_MAX 255

// path finding

#define PF_WIEGHT_MAX 250
#define PF_PATH_MOVE_LEN_MAX 250

#define BUILTIN_LED 13

#define MOTOR_A_PINS {.step = 8, .dir = 9}
#define MOTOR_B_PINS {.step = 5, .dir = 6}
#define limitY 10
#define limitX 7
#define electromagnetPin 2

#define MAX_SPEED 1000
#define NORMAL_SPEED 1000
#define SLOW_SPEED 1000

#define SWITCH_OFFSETX 60
#define SWITCH_OFFSETY 105

//van hart vak links naar hart vak rechts van speelbord is 225mm. Stap is dus 0.1mm
#define MAXBOARD_SIZEX 2250
#define MAXBOARD_SIZEY 2250

#endif
