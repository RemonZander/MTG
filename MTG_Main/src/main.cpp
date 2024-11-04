#include <LudoGame.hpp>
#include <Arduino.h>

//#define dicePin 4

LudoGame *game;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting program...");
  game->Init();
}

void loop(){
  //Serial.println("test");
  //game->GameLoop();
}