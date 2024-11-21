#include "../../src/motorDriver.hpp"
#include <stdint.h>
#include <stdio.h>

MotorDriver::MotorDriver(motorPins_t pinsMotorA, motorPins_t pinsMotorB, uint32_t endStopXPin, uint32_t endStopYPin)
    : endStopXPin(endStopXPin), endStopYPin(endStopYPin)
{
    printf("Motor Driver contructor - pinsMotorA: {step: %u, dir: %u}, pinsMotorB: {step: %u, dir: %u}, endStopXPin: %u, endStopYPin: %u\n", 
                pinsMotorA.step, pinsMotorA.dir, pinsMotorB.step, pinsMotorB.dir, endStopXPin, endStopYPin);
}

MotorDriver::~MotorDriver()
{
    printf("Motor Driver decontructor\n");
}

void MotorDriver::SetSpeeds(uint32_t maxSpeed, uint32_t acceleration, uint32_t jurk)
{
	printf("Motor Driver set speed - maxSpeed: %u, acceleration: %u, jurk: %u\n", maxSpeed, acceleration, jurk);
}

void MotorDriver::SetStepsPerMM(int32_t stepsPerMM_A, int32_t stepsPerMM_B)
{
	printf("Motor Driver set setps per mm - stepsPerMM_A: %i, stepsPerMM_B: %i\n", stepsPerMM_A, stepsPerMM_B);
}

void MotorDriver::move(float x, float y, uint32_t speed)
{
	printf("Motor Driver move - x: %f, y: %f, speed: %u\n", x, y, speed);
}

void MotorDriver::home(int32_t maxMove, uint32_t speed, float offsetX, float offsetY)
{
	printf("Motor Driver home - maxMove: %i, speed: %i, offsetX: %f, offsetY: %f\n", maxMove, speed, offsetX, offsetY);
}
