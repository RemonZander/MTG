#include "LudoUserInput.idf.hpp"
#include "UIMCommand.h"

i2c_master_bus_handle_t I2C_bus_handle = NULL;

/// @brief request the state from the UIM
/// @return the state of the UIM 
uint16_t requestState(i2c_master_dev_handle_t dev_handle) {
    uint16_t word = 0;
    while (word == 0) {
        int ret = i2c_master_receive(dev_handle, (uint8_t*)&word, sizeof(word), 1000 / portTICK_PERIOD_MS);
        if (ret != 0)
        {
            LOG_E("faild to set to 2 dice mode");
            return -1;
        }
    }
    return word;
}

LudoInputModule::LudoInputModule(uint8_t slaveAddr){
    this->slaveAddr = slaveAddr;

    if (I2C_bus_handle == NULL)
    {
        i2c_master_bus_config_t bus_config = {
            .i2c_port = I2C_NUM_0,
            .sda_io_num = GPIO_NUM_46,
            .scl_io_num = GPIO_NUM_3,
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .glitch_ignore_cnt = 7,
            .flags = {
                .enable_internal_pullup = true,
            }
        };

        ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &I2C_bus_handle));
    }

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = slaveAddr,
        .scl_speed_hz = 400000,
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(I2C_bus_handle, &dev_config, &this->dev_handle));
}

int LudoInputModule::rollDice(){
    uint8_t write_buf[2] = {this->slaveAddr, UIMCOMMAND_ENABLE_DICE_2};
    int ret = i2c_master_transmit(this->dev_handle, &write_buf[0], 2, 1000 / portTICK_PERIOD_MS);
    if (ret != 0)
    {
        LOG_E("faild to set to 2 dice mode");
        return -1;
    }

    int diceroll = 0;
    do {
        uint16_t state = requestState(this->dev_handle);
        diceroll = state >> 8;
    } while (diceroll == 0);
    delay(1000);

    write_buf[1] = UIMCOMMAND_ACKNOWLEDGE;
    ret = i2c_master_transmit(this->dev_handle, &write_buf[0], 2, 1000 / portTICK_PERIOD_MS);
    if (ret != 0)
    {
        LOG_E("faild to send acknowledge");
    }

    LOG_D("diceroll: %i", diceroll);
    return diceroll;
};

int LudoInputModule::selectPawn(){
    uint8_t write_buf[2] = {this->slaveAddr, UIMCOMMAND_ENABLE_PAWN_SELECTION};
    int ret = i2c_master_transmit(this->dev_handle, &write_buf[0], 2, 1000 / portTICK_PERIOD_MS);
    if (ret != 0)
    {
        LOG_E("faild to set to 2 dice mode");
        return -1;
    }

    int selectedPawn = 0;
    do {
        uint16_t state = requestState(this->dev_handle);
        selectedPawn = state & 0xff;
        if (selectedPawn == 4) selectedPawn = 3;
        if (selectedPawn == 8) selectedPawn = 4;            /// maurice bedenk ff comment voor dit
    } while (selectedPawn == 0);
    selectedPawn -= 1;              // so we can use this as an index

    write_buf[1] = UIMCOMMAND_ACKNOWLEDGE;
    ret = i2c_master_transmit(this->dev_handle, &write_buf[0], 2, 1000 / portTICK_PERIOD_MS);
    if (ret != 0)
    {
        LOG_E("faild to send acknowledge");
    }

    LOG_D("pown %i selected", selectedPawn);
    
    // delete[] write_buf;
    return selectedPawn;
};
