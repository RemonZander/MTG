#ifndef MOTIONCONTROLLER_H
#define MOTIONCONTROLLER_H

#include "typedefs.h"
#include <cstdint>
#include <stdint.h>

#include "motorDriver.hpp"

class MotionController
{
public:
    MotionController();
    ~MotionController();

    void SetPins(motorPins_t pinsMotorA, motorPins_t pinsMotorB, uint32_t endStopXPin, uint32_t endStopYPin);
    void SetStepsPerMM(float stepsPerMM_A, float stepsPerMM_B);
    void SetPhisicalBoardSize(float x, float y, Coordinates_t boardSize);

    bool ExecutePath(pathfinding_path_t path);

    bool MotorToPos(Coordinates_t pos);
    void SetMagnet(bool state);

private:
    MotorDriver _driver;
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
