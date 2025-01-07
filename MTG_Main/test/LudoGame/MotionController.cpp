#include "../../main/MotionController.hpp"

#include "../../main/config.h"
#include "../../main/motorDriver.hpp"
#include "../../main/typedefs.h"

#include <stdint.h>
#include <stdio.h>

MotionController::MotionController()
{
	// nothing to do
};

MotionController::~MotionController()
{
	// nothing to do
}

void MotionController::SetPins(motorPins_t pinsMotorA, motorPins_t pinsMotorB, gpio_num_t endStopXPin, gpio_num_t endStopYPin)
{
	printf("SetPins");
}

void MotionController::SetStepsPerMM(int32_t stepsPerMM_A, int32_t stepsPerMM_B)
{
	printf("SetStepsPerMM");
}

void MotionController::SetPhisicalBoardSize(float x, float y, Coordinates_t boardSize)
{
	printf("SetPhisicalBoardSize");
}

bool MotionController::ExecutePath(pathfinding_path_t path)
{
	printf("ExecutePath");
	return true;
}

void MotionController::SetMagnet(bool state)
{
	printf("SetMagnet");
}

bool MotionController::MotorToPos(Coordinates_t pos)
{
	printf("MotorToPos");
	return true;
}
