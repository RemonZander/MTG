#include "./MotionController.hpp"

#include "../../main/logger.h"
#include "../../main/config.h"
#include "../../main/typedefs.h"

MotionController::MotionController()
{
};

MotionController::~MotionController()
{
}

void MotionController::SetPhisicalBoardSize(float x, float y, float offsetX, float offsetY, Coordinates_t boardSize)
{

}

bool MotionController::ExecutePath(pathfinding_path_t path)
{

	return true;
}

void MotionController::SetMagnet(bool state)
{

}

bool MotionController::MotorToPos(Coordinates_t pos)
{

	return true;
}
