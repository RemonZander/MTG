#include "UserInputModule.hpp"
#include <Wire.h>

/// @brief The service to communicate to an User Input Module. 
/// @param slaveAddr The I2C address of the User Input Module to be communicated with.
UserInputModule::UserInputModule(int slaveAddr) : slaveAddr(slaveAddr) {
    if (!wireStarted){
        Wire.begin();
        wireStarted = true;
    };
};
bool UserInputModule::wireStarted = false;
/// @brief Sending integer to I2C slave
/// @param value the integer to send
void UserInputModule::transmit(int value) {
    Wire.beginTransmission(slaveAddr); 
    Wire.write((value&0xff00)>>8);  // write most significant byte
    Wire.write(value&0x00ff);       // write least significant byte
    Wire.endTransmission();
}

/// @brief request the state from the UIM
/// @return the state of the UIM 
int UserInputModule::requestState() {
    int word = 0;
    while (word == 0) {
        Wire.requestFrom(slaveAddr, 2);
        int index = 0;
        while (1 <= Wire.available() && index <= 1) {
            word += Wire.read() << index*8; //fix byte order (MSB first)
            index++;
        }
    }
    return word;
}
