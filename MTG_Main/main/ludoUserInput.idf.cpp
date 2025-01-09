#include <ludoUserInput.idf.hpp>
#include "UIMCommand.h"

int LudoInputModule:: rollDice(){
    //transmit(UIMCOMMAND_ENABLE_DICE_2);

    uint8_t write_buf[2] = {this.slaveAddr, UIMCOMMAND_ENABLE_DICE_2};
    i2c_master_transmit(this.dev_handle, write_buf, sizeof(write_buf), 1000 / portTICK_PERIOD_MS);

    int diceroll = 0;
    do {
        //int state = requestState();
        int state = 0;
        diceroll = state >> 8;
    } while (diceroll == 0);
    // delay(1000);
    //transmit(UIMCOMMAND_ACKNOWLEDGE);

    write_buf[2] = {this.slaveAddr, UIMCOMMAND_ACKNOWLEDGE};
    i2c_master_transmit(this.dev_handle, write_buf, sizeof(write_buf), 1000 / portTICK_PERIOD_MS);

    LOG_D("diceroll: %i", diceroll);

    delete[] write_buf;
    return diceroll;
};

int LudoInputModule:: selectPawn(){
    //transmit(UIMCOMMAND_ENABLE_PAWN_SELECTION);

    uint8_t write_buf[2] = {this.slaveAddr, UIMCOMMAND_ENABLE_PAWN_SELECTION};
    i2c_master_transmit(this.dev_handle, write_buf, sizeof(write_buf), 1000 / portTICK_PERIOD_MS);

    int selectedPawn = 0;
    do {
        //int state = requestState();
        int state = 0;
        selectedPawn = state & 0xff;
        if (selectedPawn == 4) selectedPawn = 3;
        if (selectedPawn == 8) selectedPawn = 4;            /// maurice bedenk ff comment voor dit
    } while (selectedPawn == 0);
    selectedPawn -= 1;              // so we can use this as an index
    // delay(100);
    //transmit(UIMCOMMAND_ACKNOWLEDGE);

    write_buf[2] = {this.slaveAddr, UIMCOMMAND_ACKNOWLEDGE};
    i2c_master_transmit(this.dev_handle, write_buf, sizeof(write_buf), 1000 / portTICK_PERIOD_MS);

    LOG_D("pown %i selected", selectedPawn);
    
    delete[] write_buf;
    return selectedPawn;
};