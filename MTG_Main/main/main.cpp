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

#ifdef ARDUINO
void loop() {}
void setup()
#else
extern "C" void app_main(void)
#endif
{
#ifdef ARUINO
  Serial.begin(115200);
#endif
  
  game->Init();

  game->GameLoop();
}
