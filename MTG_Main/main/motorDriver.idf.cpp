#include "motorDriver.hpp"
#include <stdint.h>


MotorDriver::MotorDriver(motorPins_t pinsMotorA, motorPins_t pinsMotorB, uint32_t endStopXPin, uint32_t endStopYPin)
    : endStopXPin(endStopXPin), endStopYPin(endStopYPin)
{
}

MotorDriver::~MotorDriver()
{
}

void MotorDriver::SetSpeeds(uint32_t maxSpeed, uint32_t acceleration, uint32_t jurk)
{
}


void MotorDriver::SetStepsPerMM(int32_t a, int32_t b)
{
    stepsPerMMA = a;
    stepsPerMMB = b;
}

void MotorDriver::move(float x, float y, uint32_t speed)
{
    int32_t moveA = -y + x;
    int32_t moveB = -y - x;
}

void MotorDriver::home(int32_t maxMove, uint32_t speed, float offsetX, float offsetY)
{
}
