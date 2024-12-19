#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <stdint.h>
#include "typedefs.h"

#ifdef ARDUINO
#include <AccelStepper.h>
#endif

#ifdef IDF_VER
#include <soc/gpio_num.h>
#endif
struct motorPins_t {
    gpio_num_t step;
    gpio_num_t dir;
};

class MotorDriver {
public:
    MotorDriver(motorPins_t pinsMotorA, motorPins_t pinsMotorB, gpio_num_t endStopXPin, gpio_num_t endStopYPin);
    ~MotorDriver();

    /** SetSpeed
     * 
     * args:
     * - maxspeed: maximum allowed speed in mm/sec
     * - accelleration: avearage acceleration in mm/sec^2 (a smoothening function is applaied over the acceleraton. Thus the max aceleration will be slightly higher)
     * - jurk: start speed without accelaration in mm/sec
     */
    int SetSpeeds(uint32_t maxSpeed, uint32_t acceleration, uint32_t jurk);

    void SetStepsPerMM(int32_t a, int32_t b);

    /** move
     * move the motors relitivly
     * 
     * args:
     * - deltaA: number of steps to move in A motor
     * - deltaB: number of steps to move in B motor
     * - speed: speed of the motors in steps/sec
     */
    void move(float x, float y, uint32_t speed);

    /** home
     * Move to endstops (home position)
     * 
     * args:
     * - maxMove: max number of steps to set during homeing
     * - speed: speed of the motors in steps/sec
     */
    void home(int32_t maxMove, uint32_t speed, float offsetX, float offsetY);

private:
    gpio_num_t endStopXPin, endStopYPin;
    motorPins_t stepperAPins, stepperBPins;
    int32_t stepsPerMMA, stepsPerMMB;
    uint32_t acceleration_steps;

    uint32_t jurk = 0;
};

#endif
