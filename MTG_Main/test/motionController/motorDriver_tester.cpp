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
    printf("Motor Driver decontructor");
}

void MotorDriver::SetSpeeds(uint32_t maxSpeed, uint32_t acceleration, uint32_t jurk)
{
	printf("Motor Driver set speed - maxSpeed: %u, acceleration: %u, jurk: %u", maxSpeed, acceleration, jurk);
}

void MotorDriver::move(int32_t deltaA, int32_t deltaB, uint32_t speed)
{
	printf("Motor Driver move - deltaA: %i, deltaB: %i, speed: %u", deltaA, deltaB, speed);
}

void MotorDriver::home(int32_t maxMove, uint32_t speed)
{
	printf("Motor Driver home - maxMove: %i, speed: %i", maxMove, speed);
}
