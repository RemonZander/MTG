#ifndef CONFIG_H
#define CONFIG_H

#include "typedefs.h"

#define LOGLEVEL_DEBUG 5
#define LOGLEVEL_INFO 4
#define LOGLEVEL_WARN 3
#define LOGLEVEL_ERROR 2
#define LOGLEVEL_CRITICAL 1

#define LOGLEVEL LOGLEVEL_DEBUG

#define BOARD_SIZE_X_MAX 20
#define BOARD_SIZE_Y_MAX 20

#define BOARD_SIZE_X_LUDO 15
#define BOARD_SIZE_Y_LUDO 15

// path finding

#define PF_WIEGHT_MAX 250
#define PF_PATH_MOVE_LEN_MAX 250

#define BUILTIN_LED 13

#define MOTOR_A_PINS {.step = GPIO_NUM_8, .dir = GPIO_NUM_9}
#define MOTOR_B_PINS {.step = GPIO_NUM_5, .dir = GPIO_NUM_6}
#define STEPER_DIR_CCW 1
#define STEPER_DIR_CW 0
#define STEPS_PER_MM 10
#define limitY GPIO_NUM_7
#define limitX GPIO_NUM_10
#define electromagnetPin GPIO_NUM_2

#define MOTOR_MAX_SPEED 500
#define MOTOR_ACCELARATION 500
#define MOTOR_HOME_SPEED 250
#define MOTOR_HOME_ACCELARATION 10000

#define SWITCH_OFFSETX 60
#define SWITCH_OFFSETY 105

#define MAXBOARD_SIZEX 225.0
#define MAXBOARD_SIZEY 225.0

#endif
