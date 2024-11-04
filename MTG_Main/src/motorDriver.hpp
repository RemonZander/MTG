#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <stdint.h>

#include <AccelStepper.h>

struct motorPins_t {
    uint16_t step;
    uint16_t dir;
};

class MotorDriver {
public:
    MotorDriver(motorPins_t pinsMotorA, motorPins_t pinsMotorB, uint32_t endStopXPin, uint32_t endStopYPin);
    ~MotorDriver();

    /** SetSpeed
     * 
     * args:
     * - maxspeed: maximum allowed speed in steps/sec
     * - accelleration: in steps/sec^2
     * - jurk: start speed without accelaration (not suported)
     */
    void SetSpeeds(uint32_t maxSpeed, uint32_t acceleration, uint32_t jurk);

    /** move
     * move the motors relitivly
     * 
     * args:
     * - deltaA: number of steps to move in A motor
     * - deltaB: number of steps to move in B motor
     * - speed: speed of the motors in steps/sec
     */
    void move(int32_t deltaA, int32_t deltaB, uint32_t speed);

    /** home
     * Move to endstops (home position)
     * 
     * args:
     * - maxMove: max number of steps to set during homeing
     * - speed: speed of the motors in steps/sec
     */
    void home(int32_t maxMove, uint32_t speed);

private:
    AccelStepper *stepperMotorA = NULL;
    AccelStepper *stepperMotorB = NULL;
    uint32_t endStopXPin, endStopYPin;

    uint32_t jurk = 0;
};

#endif
