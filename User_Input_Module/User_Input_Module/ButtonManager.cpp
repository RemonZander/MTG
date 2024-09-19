#include "ButtonManager.hpp"
#include "Arduino.h"

ButtonManager::ButtonManager() {
  pinMode(UIMBUTTON_LED_DICEROLL, OUTPUT);
  pinMode(UIMBUTTON_LED_PAWNS, OUTPUT);
  pinMode(UIMBUTTON_DICEROLL, INPUT_PULLUP);
  pinMode(UIMBUTTON_PAWN_1, INPUT_PULLUP);
  pinMode(UIMBUTTON_PAWN_2, INPUT_PULLUP);
  pinMode(UIMBUTTON_PAWN_3, INPUT_PULLUP);
  pinMode(UIMBUTTON_PAWN_4, INPUT_PULLUP);
}

uimButton ButtonManager::buttonPressed() {
  for (int i = UIMBUTTON_DICEROLL; i <= UIMBUTTON_PAWN_4; i++) {
    if (digitalRead(i) == 0)
      return i;
  }
  return 0;
}

void ButtonManager::toggleLed(uimButtonLED led, bool state) {
  digitalWrite(led, state);
}