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

#define BOARD_SIZE_X_MAX 20
#define BOARD_SIZE_Y_MAX 20

#define BOARD_SIZE_X_LUDO 15
#define BOARD_SIZE_Y_LUDO 15

// path finding

#define PF_WIEGHT_MAX 250
#define PF_PATH_MOVE_LEN_MAX 250

#define BUILTIN_LED 13

#define MOTOR_A_STEP_PIN GPIO_NUM_12
#define MOTOR_A_DIR_PIN GPIO_NUM_11
#define MOTOR_B_STEP_PIN GPIO_NUM_14
#define MOTOR_B_DIR_PIN GPIO_NUM_13
#define STEPER_DIR_CCW 1
#define STEPER_DIR_CW 0
#define STEPS_PER_MM 10
#define LIMIT_Y_PIN GPIO_NUM_9
#define LIMIT_X_PIN GPIO_NUM_10
#define electromagnetPin GPIO_NUM_2

#define MOTOR_MAX_SPEED 20
#define MOTOR_JURK 0.5
#define MOTOR_ACCELARATION 3
#define MOTOR_HOME_SPEED 250
#define MOTOR_HOME_ACCELARATION 10000

#define SWITCH_OFFSETX 60
#define SWITCH_OFFSETY 105

#define MAXBOARD_SIZEX 225.0
#define MAXBOARD_SIZEY 225.0

#define delay(ms) vTaskDelay(pdMS_TO_TICKS(ms))

#endif
