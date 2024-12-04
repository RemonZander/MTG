#include "UserInputModule.hpp"

#ifdef ARUDINO
#include <Wire.h>
#endif

/// @brief The service to communicate to an User Input Module. 
/// @param slaveAddr The I2C address of the User Input Module to be communicated with.
UserInputModule::UserInputModule(int slaveAddr) : slaveAddr(slaveAddr) {
#ifdef ARUDINO
    if (!wireStarted){
        Wire.begin();
        wireStarted = true;
    };
#endif
};
bool UserInputModule::wireStarted = false;
/// @brief Sending integer to I2C slave
/// @param value the integer to send
void UserInputModule::transmit(int value) {
#ifdef ARUDINO
    Wire.beginTransmission(slaveAddr); 
    Wire.write((value&0xff00)>>8);  // write most significant byte
    Wire.write(value&0x00ff);       // write least significant byte
    Wire.endTransmission();
#endif
}

/// @brief request the state from the UIM
/// @return the state of the UIM 
int UserInputModule::requestState() {
    int word = 0;
#ifdef ARUDINO
    while (word == 0) {
        Wire.requestFrom(slaveAddr, 2);
        int index = 0;
        while (1 <= Wire.available() && index <= 1) {
            word += Wire.read() << index*8; //fix byte order (MSB first)
            index++;
        }
    }
#endif
    return word;
}
