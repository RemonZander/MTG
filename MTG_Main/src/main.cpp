#include <LudoGame.hpp>
#include <Arduino.h>

//#define dicePin 4

LudoGame *game;

void setup()
{
  Serial.begin(9600);
  game->Init();
}

void loop(){
  game->GameLoop();
}
  	
// void(* resetFunc) (void) = 0; //declare reset function @ address 0

