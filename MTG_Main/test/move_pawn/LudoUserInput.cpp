#include "./LudoUserInput.hpp"

/// @brief request the state from the UIM
/// @return the state of the UIM 
uint16_t requestState() {
    uint16_t word = 0;

    return word;
};

LudoInputModule::LudoInputModule(uint8_t slaveAddr){
    this->slaveAddr = slaveAddr;
};

int LudoInputModule::rollDice(){
    return 0;
};

int LudoInputModule::selectPawn(){
    return 0;
};
