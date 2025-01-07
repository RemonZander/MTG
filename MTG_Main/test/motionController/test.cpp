#include <gtest/gtest.h>
#include <stdio.h>

#include "../../main/MotionController.hpp"


TEST(MotionController, init)
{
	MotionController *controller = new MotionController();
	controller->SetPins({.step = 0, .dir = 0}, {.step = 0, .dir = 0}, 0, 0);
	controller->SetPhisicalBoardSize(250.0, 250.0, {.x = 10, .y = 10});
	delete(controller);
}