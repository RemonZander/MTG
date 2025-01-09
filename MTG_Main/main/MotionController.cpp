#include "MotionController.hpp"

#include "config.h"
#include "motorDriver.hpp"
#include "typedefs.h"

MotionController::MotionController()
{
	this->_driver = new MotorDriver();
	this->_driver->init();
};

MotionController::~MotionController()
{
	delete(this->_driver);
}

void MotionController::SetPins()
{
	this->_driver = new MotorDriver();
	this->_driver->init();
}

void MotionController::SetStepsPerMM(int32_t stepsPerMM_A, int32_t stepsPerMM_B)
{
}

void MotionController::SetPhisicalBoardSize(float x, float y, Coordinates_t boardSize)
{
	this->_phisicalBoardSizeX = x;
	this->_phisicalBoardSizeY = y;
	this->_squareSize_x = x / boardSize.x;
	this->_squareSize_y = y / boardSize.y;
	this->_boardOffset_x = 125.0 - (x/2) + (_squareSize_x/2);
	this->_boardOffset_y = 118.0 - (y/2) + (_squareSize_y/2);
}

bool MotionController::ExecutePath(pathfinding_path_t path)
{
	for (std::size_t i=0; i < path->size(); i++)
	{
		pathfinding_step_t step = path->at(i);
		this->SetMagnet(step.magnetEn);
		this->MotorToPos(step.target);
	}
	return true;
}

void MotionController::SetMagnet(bool state)
{
	this->_driver->setMagnet(state);
}

bool MotionController::MotorToPos(Coordinates_t pos)
{
	if (pos.x > BOARD_SIZE_X_MAX || pos.y > BOARD_SIZE_X_MAX)
	{
		return false;
	}

	float deltaMM_X = ((float)(pos.x - _currPos.x)) * _squareSize_x;
	float deltaMM_Y = ((float)(pos.y - _currPos.y)) * _squareSize_y;

	this->_driver->move(deltaMM_X, deltaMM_Y, 100);

	this->_currPos = pos;
	return true;
}
