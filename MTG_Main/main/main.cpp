#include "config.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <stdio.h>
#include <esp_task_wdt.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif

// #include <ludoGame.hpp>
#include <motorDriver.hpp>
#include "logger.h"

// LudoGame* game;

#ifdef ARDUINO
void loop() {}
void setup()
#else
extern "C" void app_main(void)
#endif
{
#ifdef ARUINO
  Serial.begin(115200);
#else
	// disable watchdog
	ESP_ERROR_CHECK(esp_task_wdt_deinit());
#endif
  
  // game->Init();

  // game->GameLoop();

  MotorDriver *md = new MotorDriver();
  md->init();

  // LOG_D("wait");

  // delay(5000);
  LOG_D("move");
  
  md->move(10.0, -0.0, 100);
  LOG_D("bloep");
  while(true);
}
