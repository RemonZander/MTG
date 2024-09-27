#include "LudoBoard.hpp"
#include "pawn.hpp"
#include "player.hpp"
#include "LudoUserInput.hpp"
#include "LudoGame.hpp"

#define dicePin 4

LudoGame *game;

void setup()
{

  Serial.begin(9600);
  pinMode(dicePin, INPUT_PULLUP);
  pinMode(7, INPUT);
  pinMode(10, INPUT);
  Serial.println("board started");

  game = new LudoGame(4);
}

void loop(){
  game->nextTurn();
  delay(200);
}
  	
// void(* resetFunc) (void) = 0; //declare reset function @ address 0

