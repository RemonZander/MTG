#include "config.h"

#include "MotionController.hpp"

MotionController *motion;

void setup()
{
  Serial.begin(9600);
  Serial.println("board started V0.0.4");


  motion = new MotionController();
  motion->SetPins(MOTOR_A_PINS, MOTOR_B_PINS, limitX, limitY);
  motion->SetPhisicalBoardSize(1000.0, 1000.0, {.x = 10, .y = 10});

  motion->MotorToPos({.x = 5, .y = 5});
}

void loop(){
}

// void(* resetFunc) (void) = 0; //declare reset function @ address 0

