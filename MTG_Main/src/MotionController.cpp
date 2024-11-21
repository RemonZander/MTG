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
	_driver->home(2500, 100, _boardOffset_x, _boardOffset_y);
	_driver->SetSpeeds(MOTOR_MAX_SPEED, MOTOR_ACCELARATION, 0);
}

void MotionController::SetStepsPerMM(int32_t stepsPerMM_A, int32_t stepsPerMM_B)
{
	_driver->SetStepsPerMM(stepsPerMM_A, stepsPerMM_B);
}

void MotionController::SetPhisicalBoardSize(float x, float y, Coordinates_t boardSize)
{
	_phisicalBoardSizeX = x;
	_phisicalBoardSizeY = y;
	_squareSize_x = x / boardSize.x;
	_squareSize_y = y / boardSize.y;
	_boardOffset_x = x / (boardSize.x*2);
	_boardOffset_y = y / (boardSize.y*2);
}

bool MotionController::ExecutePath(pathfinding_path_t path)
{
	for (size_t i=0; i < path->size(); i++)
	{
		pathfinding_step_t step = path->at(i);
		SetMagnet(step.magnetEn);
		MotorToPos(step.target);
	}
	return true;
}

void MotionController::SetMagnet(bool state)
{
	#ifdef ARDUINO
	digitalWrite(electromagnetPin, state);
	digitalWrite(BUILTIN_LED, state);
	#endif
}

bool MotionController::MotorToPos(Coordinates_t pos)
{
	if (pos.x > BOARD_SIZE_X_MAX || pos.y > BOARD_SIZE_X_MAX)
	{
		return false;
	}

	float deltaMM_X = ((float)(pos.x - _currPos.x)) * _squareSize_x;
	float deltaMM_Y = ((float)(pos.y - _currPos.y)) * _squareSize_y;

	_driver->move(deltaMM_X, deltaMM_Y, 100);

	_currPos = pos;
	return true;
}
