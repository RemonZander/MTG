#ifndef CONFIG_H
#define CONFIG_H

#define LOGLEVEL "DEBUG" // "DEBUG", "INFO", "WARN", "ERROR", "CRITICAL"

#define BOARD_SIZE_X_MAX 20
#define BOARD_SIZE_Y_MAX 20

#define BOARD_SIZE_X_LUDO 15
#define BOARD_SIZE_Y_LUDO 15

// path finding

#define PF_WIEGHT_MAX 250
#define PF_PATH_MOVE_LEN_MAX 250

#define BUILTIN_LED 13

#define MOTOR_A_PINS {.step = 8, .dir = 9}
#define MOTOR_B_PINS {.step = 5, .dir = 6}
#define STEPS_PER_MM 10
#define limitY 7
#define limitX 10
#define electromagnetPin 2

#define MOTOR_MAX_SPEED 500
#define MOTOR_ACCELARATION 500
#define MOTOR_HOME_SPEED 250
#define MOTOR_HOME_ACCELARATION 10000

#define SWITCH_OFFSETX 60
#define SWITCH_OFFSETY 105

#define MAXBOARD_SIZEX 225.0
#define MAXBOARD_SIZEY 225.0

#endif
