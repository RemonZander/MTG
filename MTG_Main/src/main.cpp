#include "config.h"

#include "PathFinding.hpp"
#include "MotionController.hpp"

MotionController *motion;

PathFinding_impoved pathFind = PathFinding_impoved();
BoardMap_t wightMap = {
    .size = {.x = 15, .y = 15},
    .map = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    }
};
Cordinates_s start = { .x = 2, .y = 2 };
Cordinates_s end = { .x = 7, .y = 7 };
std::vector<Cordinates_s> *powns = new std::vector<Cordinates_s>;
pathfinding_path_t path;

void setup()
{
  Serial.begin(9600);
  Serial.println("board started V0.0.9");

  motion = new MotionController();
  motion->SetPhisicalBoardSize(240.0, 240.0, {.x = 15, .y = 15});
  motion->SetStepsPerMM(STEPS_PER_MM, STEPS_PER_MM);
  motion->SetPins(MOTOR_A_PINS, MOTOR_B_PINS, limitX, limitY);

  Serial.println("init done");
  delay(2000);
  Serial.println("move to ( 1, 1)");

  motion->MotorToPos({.x = 1, .y = 1});
  delay(2000);
  Serial.println("move to (14,14)");

  motion->MotorToPos({.x = 14, .y = 14});

  delay(2000);
  motion->MotorToPos(start);

  powns->push_back({.x = 1, .y = 7});
  powns->push_back({.x = 2, .y = 6});
  powns->push_back({.x = 3, .y = 5});
  // powns->push_back({.x = 4, .y = 4});
  powns->push_back({.x = 5, .y = 3});
  powns->push_back({.x = 6, .y = 2});
  powns->push_back({.x = 7, .y = 1});

  path = pathFind.findPath(start, end, wightMap, powns, 5);
  motion->ExecutePath(path);
  // delete(path);
}

void loop(){
}

