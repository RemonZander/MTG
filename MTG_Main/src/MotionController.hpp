#ifndef MOTIONCONTROLLER_H
#define MOTIONCONTROLLER_H

#include "typedefs.h"
#include <stdint.h>

#include "motorDriver.hpp"

class MotionController
{
public:
    MotionController();
    ~MotionController();

    /**
    * @brief Sets the pins for the motors and end stops.
    *
    * This function initializes the pins for the motors and end stops. It takes four parameters:
    * - pinsMotorA: A structure containing the pin numbers for motor A.
    * - pinsMotorB: A structure containing the pin numbers for motor B.
    * - endStopXPin: The pin number for the X-axis end stop.
    * - endStopYPin: The pin number for the Y-axis end stop.
    *
    * @param pinsMotorA A structure containing the pin numbers for motor A.
    * @param pinsMotorB A structure containing the pin numbers for motor B.
    * @param endStopXPin The pin number for the X-axis end stop.
    * @param endStopYPin The pin number for the Y-axis end stop.
    *
    * @return void
    */
    void SetPins(motorPins_t pinsMotorA, motorPins_t pinsMotorB, uint32_t endStopXPin, uint32_t endStopYPin);

    /**
    * @brief Sets the steps per millimeter for both motors.
    *
    * This function sets the number of steps per millimeter for both motors A and B.
    * It is used to calculate the required steps for moving the motors to specific positions.
    *
    * @param stepsPerMM_A The number of steps per millimeter for motor A.
    * @param stepsPerMM_B The number of steps per millimeter for motor B.
    *
    * @return void
    */
    void SetStepsPerMM(float stepsPerMM_A, float stepsPerMM_B);
    void SetPhisicalBoardSize(float x, float y, Coordinates_t boardSize);

    bool ExecutePath(pathfinding_path_t path);

    bool MotorToPos(Coordinates_t pos);
    void SetMagnet(bool state);

private:
    MotorDriver *_driver;
    Coordinates_t _boardSize = {0, 0};
    float _phisicalBoardSizeX = 0.0;
    float _phisicalBoardSizeY = 0.0;
    Coordinates_t _currPos = {0, 0};
    float _stepsPerMM_A = 0.0;
    float _stepsPerMM_B = 0.0;
    float _squareSize_x = 0.0;
    float _squareSize_y = 0.0;
};

#endif
