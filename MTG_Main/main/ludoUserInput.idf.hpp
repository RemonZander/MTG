#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

class LudoInputModule {
    public:
        LudoInputModule(int masterAddr, int slaveAddr){
            this.slaveAddr = slaveAddr;
            this.bus_config = {
                .i2c_port = masterAddr,
                .sda_io_num = 46,
                .scl_io_num = 3,
                .clk_source = I2C_CLK_SRC_DEFAULT,
                .glitch_ignore_cnt = 7,
                .flags.enable_internal_pullup = true,
            };

            ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, this.bus_handle));
            
            this.dev_config = {
                .dev_addr_length = I2C_ADDR_BIT_LEN_7,
                .device_address = slaveAddr,
                .scl_speed_hz = CONFIG_I2C_MASTER_FREQUENCY,
            };

            ESP_ERROR_CHECK(i2c_master_bus_add_device(*this.bus_handle, &dev_config, this.dev_handle));
        };
        int rollDice();
        int selectPawn();
    
    private:
        i2c_master_bus_config_t bus_config;
        i2c_device_config_t dev_config;
        i2c_master_bus_handle_t bus_handle;
        i2c_master_dev_handle_t dev_handle;
        int slaveAddr;
};

#endif