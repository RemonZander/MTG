#include "MotionController.hpp"

#include "config.h"
#include "motorDriver.hpp"
#include "typedefs.h"

MotionController::MotionController()
{
	// nothing to do
};

MotionController::~MotionController()
{
	// nothing to do
}

void MotionController::SetPins(motorPins_t pinsMotorA, motorPins_t pinsMotorB, uint32_t endStopXPin, uint32_t endStopYPin)
{
	_driver = new MotorDriver(pinsMotorA, pinsMotorB, endStopXPin, endStopYPin);
	_driver->SetSpeeds(MOTOR_HOME_SPEED, MOTOR_HOME_ACCELARATION, 0);
	_driver->home(1000, 100);
	_driver->SetSpeeds(MOTOR_MAX_SPEED, MOTOR_ACCELARATION, 0);
}

void MotionController::SetStepsPerMM(float stepsPerMM_A, float stepsPerMM_B)
{
	_stepsPerMM_A = stepsPerMM_A;
	_stepsPerMM_B = stepsPerMM_B;
}

void MotionController::SetPhisicalBoardSize(float x, float y, Coordinates_t boardSize)
{
	_phisicalBoardSizeX = x;
	_phisicalBoardSizeY = y;
	_squareSize_x = x / boardSize.x;
	_squareSize_y = y / boardSize.y;
}

// bool MotionController::ExecutePath(pathfinding_path_t path)
bool MotionController::ExecutePath(pathfinding_step_t* path, uint16_t pathLen)
{
	for (uint16_t i=0; i < pathLen; i++)
	{
		pathfinding_step_t step = *(path + i);
		SetMagnet(step.magnetEn);
		MotorToPos(step.target);
	}
}

void MotionController::SetMagnet(bool state)
{
	digitalWrite(electromagnetPin, state);
	digitalWrite(BUILTIN_LED, state);
}

bool MotionController::MotorToPos(Coordinates_t pos)
{
	if (pos.x > BOARD_SIZE_X_MAX || pos.y > BOARD_SIZE_X_MAX)
	{
		return false;
	}

	float deltaMM_X = ((float)(pos.x - _currPos.x)) * _squareSize_x;
	float deltaMM_Y = ((float)(pos.y - _currPos.y)) * _squareSize_y;

	float deltaA = -deltaMM_Y + deltaMM_X;
	float deltaB = -deltaMM_Y - deltaMM_X;

	_driver->move(deltaA * _stepsPerMM_A, deltaB * _stepsPerMM_B, 100);

	_currPos = pos;
	return true;
}
