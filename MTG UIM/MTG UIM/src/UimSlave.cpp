#include "UimSlave.hpp"
#include "Arduino.h"
#include <Wire.h>

void (*functionToCall)(int command);
static int *modState;

void UimSlave::requestEvent() {
  Wire.write(highByte(*modState));
  Wire.write(lowByte(*modState));
}

void UimSlave::receiveEvent() {
  byte highByte;
  byte lowByte;
  int index = 0;
  while (1 <= Wire.available()) {
    if (index == 0)
      highByte = Wire.read();
    else if (index == 1)
      lowByte = Wire.read();
    index++;
  }
  functionToCall(word(highByte, lowByte));
}

void UimSlave::begin(const int address, void *parseFunction, int *state) {
  Wire.begin(address);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  functionToCall = parseFunction;
  modState = state;
}
