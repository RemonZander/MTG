#include "config.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <stdio.h>
#include <esp_task_wdt.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif

#include <ludoGame.hpp>
// #include <motorDriver.hpp>
#include "logger.h"

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
#else
	// disable watchdog
	ESP_ERROR_CHECK(esp_task_wdt_deinit());
#endif
  
  game->Init();

  game->GameLoop();

  // MotorDriver *md = new MotorDriver(
  //   // {.step = GPIO_NUM_12, .dir = GPIO_NUM_11},
  //   {.step = GPIO_NUM_20, .dir = GPIO_NUM_11},
  //   // {.step = GPIO_NUM_14, .dir = GPIO_NUM_13},
  //   {.step = GPIO_NUM_19, .dir = GPIO_NUM_13},
  //   limitY,
  //   limitX
  // );

  // LOG_D("wait");

  // delay(5000);
  // LOG_D("move");
  
  // md->move(1.0, -1.0, 100);
  // while(true);
}
