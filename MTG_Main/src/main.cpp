#include "config.h"
#include <ludoGame.hpp>

LudoGame* game;

void setup()
{
  Serial.begin(9600);
  
  game->Init();

  game->GameLoop();
}

void loop(){
}

