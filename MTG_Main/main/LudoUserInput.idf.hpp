#ifndef LUDOINPUTMODULE_HPP
#define LUDOINPUTMODULE_HPP

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "logger.h"

class LudoInputModule {
    public:
        LudoInputModule(uint8_t slaveAddr);
        int rollDice();
        int selectPawn();
    
    private:
        i2c_master_dev_handle_t dev_handle;
        uint8_t slaveAddr;
};

#endif