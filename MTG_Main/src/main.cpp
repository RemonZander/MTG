#include "config.h"
#include <ludoGame.hpp>

#include "MotionController.hpp"

LudoGame* game;

void setup()
{
  Serial.begin(9600);
  
  game->Init();

  game->GameLoop();
}

void loop(){
}

// void(* resetFunc) (void) = 0; //declare reset function @ address 0

