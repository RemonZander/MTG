#ifndef CONFIG_H
#define CONFIG_H

//temp fix for GPIO pins
#ifndef IDF_VER
#define GPIO_NUM_12 0
#define GPIO_NUM_11 1
#define GPIO_NUM_14 2
#define GPIO_NUM_13 3
#define GPIO_NUM_9 4
#define GPIO_NUM_10 5
#endif

#define LOGLEVEL_DEBUG 5
#define LOGLEVEL_INFO 4
#define LOGLEVEL_WARN 3
#define LOGLEVEL_ERROR 2
#define LOGLEVEL_CRITICAL 1

#define LOGLEVEL LOGLEVEL_DEBUG

#define delay(ms) vTaskDelay(pdMS_TO_TICKS(ms))

// max board size in squares to support (not het board size used)
#define BOARD_SIZE_X_MAX 20
#define BOARD_SIZE_Y_MAX 20

// board size in squares used for LUDO
#define BOARD_SIZE_X_LUDO 15
#define BOARD_SIZE_Y_LUDO 15

// ========================
// === path finding =======
// ========================

// max total path wieght. when this wieght is reacht it is assumed no path available
#define PF_WIEGHT_MAX 250
// max total staps in path. when this wieght is reacht it is assumed no path available
#define PF_PATH_MOVE_LEN_MAX 250


// ========================
// === motor driver =======
// ========================

// pins for motor a. motor to the front of the machine, connected to the lower motor dirver
#define MOTOR_A_STEP_PIN GPIO_NUM_12
#define MOTOR_A_DIR_PIN GPIO_NUM_11
// pins for motor b. motor close to the power plug, connected to the upper motor dirver
#define MOTOR_B_STEP_PIN GPIO_NUM_14
#define MOTOR_B_DIR_PIN GPIO_NUM_13
// reverse motors. 1 for reverse, 0 for forward
#define MOTOR_A_REVERSE 0
#define MOTOR_B_REVERSE 0
// steps per mm
#define STEPS_PER_MM 40
// pins for limit switches
#define LIMIT_Y_PIN GPIO_NUM_9
#define LIMIT_X_PIN GPIO_NUM_10
// pin for magnet
#define MAGNET_PIN GPIO_NUM_21

// jurk in mm/s. the start speed without acceleration
#define MOTOR_JURK 33
// max speed to move in mm/s
#define MOTOR_MAX_SPEED 35
// acceleration in mm/s^2
#define MOTOR_ACCELARATION 25
// homeing speed in mm/s (homeing has no acceleration)
#define MOTOR_HOME_SPEED 30

// max board size in mm
#define MAXBOARD_SIZEX 240.0
#define MAXBOARD_SIZEY 240.0

#define MODE_SW_PIN GPIO_NUM_41
#define MODE_JP_PIN GPIO_NUM_42

#endif
