#include "motorDriver.hpp"
#include <cstdint>

MotorDriver::MotorDriver(motorPins_t pinsMotorA, motorPins_t pinsMotorB, uint32_t endStopXPin, uint32_t endStopYPin)
    : endStopXPin(endStopXPin), endStopYPin(endStopYPin)
{
    stepperMotorA = new AccelStepper(1, pinsMotorA.step, pinsMotorA.dir);
    stepperMotorB = new AccelStepper(1, pinsMotorB.step, pinsMotorB.dir);

    pinMode(endStopXPin, INPUT);
    pinMode(endStopYPin, INPUT);
}

MotorDriver::~MotorDriver()
{
    delete(stepperMotorA);
    delete(stepperMotorB);
}

void MotorDriver::setSpeeds(uint32_t maxSpeed, uint32_t acceleration, uint32_t jurk)
{
	stepperMotorA->setMaxSpeed(maxSpeed);
	stepperMotorA->setAcceleration(acceleration);

	stepperMotorB->setMaxSpeed(maxSpeed);
	stepperMotorB->setAcceleration(acceleration);

    this->jurk = jurk;
}

void MotorDriver::move(int32_t deltaA, int32_t deltaB, uint32_t speed)
{
	stepperMotorA->setSpeed(speed);
	stepperMotorB->setSpeed(speed);

	stepperMotorA->moveTo(deltaA);
	stepperMotorB->moveTo(deltaB);

    bool motorAFinished = false, motorBFinished = false;

    while (!motorAFinished && !motorBFinished)
    {
        motorAFinished = stepperMotorA->run();
        motorBFinished = stepperMotorB->run();
    }
}

void MotorDriver::home(int32_t maxMove, uint32_t speed)
{
	stepperMotorA->setSpeed(speed);
	stepperMotorB->setSpeed(speed);

	stepperMotorA->moveTo(maxMove);
	stepperMotorB->moveTo(-maxMove);

    bool motorAFinished = false, motorBFinished = false;

    while (!motorAFinished && !motorBFinished && (digitalRead(endStopXPin) == 1))
    {
        motorAFinished = stepperMotorA->run();
        motorBFinished = stepperMotorB->run();
    }

	stepperMotorA->moveTo(maxMove);
	stepperMotorB->moveTo(-maxMove);

    motorAFinished = false;
    motorBFinished = false;

    while (!motorAFinished && !motorBFinished && (digitalRead(endStopYPin) == 1))
    {
        motorAFinished = stepperMotorA->run();
        motorBFinished = stepperMotorB->run();
    }
}
