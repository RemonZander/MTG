#include "config.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif

#include <ludoGame.hpp>

LudoGame* game;

void setup()
{
  Serial.begin(9600);
  
  game->Init();

  game->GameLoop();
}
