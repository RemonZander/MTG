#include "DiceService.hpp"
#include "UIMDiceLed.h"
#include "Arduino.h"

DiceService::DiceService() {
    pinMode(UIMSHIFT_CLOCK, OUTPUT);
    pinMode(UIMSHIFT_DATA, OUTPUT);  
    pinMode(UIMSHIFT_LATCH, OUTPUT);
    updateShiftRegister();
}

void DiceService::updateShiftRegister() {
   digitalWrite(UIMSHIFT_LATCH, LOW);
   shiftOut(UIMSHIFT_DATA, UIMSHIFT_CLOCK, LSBFIRST, ledState);
   digitalWrite(UIMSHIFT_LATCH, HIGH);
}

void DiceService::resetDice() {
    ledState = 0;
    updateShiftRegister();
}

int DiceService::generateNumber(bool twoDice) {
    int randomNo1 = random(1, 7);
    while (randomNo1 == lastThrown1) {
        randomNo1 = random(1, 7);
    }
    lastThrown1 = randomNo1;

    int randomNo2 = random(1, 7);
    while (randomNo2 == lastThrown2) {
        randomNo2 = random(1, 7);
    }
    lastThrown2 = randomNo2;
    
    resetDice();
    showNumber(1, randomNo1);
    if (twoDice) {
        showNumber(2, randomNo2);
        return randomNo1 + randomNo2;
    }
    return randomNo1;
}

void DiceService::showNumber(int diceNumber, int number) {
    // Dice 1
    if (diceNumber == 1) {
        switch (number) {
            case (1):
                bitSet(ledState, UIMDICE_1_LED_4);
                break;
            case (2):
                bitSet(ledState, UIMDICE_1_LED_1);
                break;
            case (3):
                bitSet(ledState, UIMDICE_1_LED_1);
                bitSet(ledState, UIMDICE_1_LED_4);
                break;
            case (4):
                bitSet(ledState, UIMDICE_1_LED_1);
                bitSet(ledState, UIMDICE_1_LED_2);
                break;
            case (5):
                bitSet(ledState, UIMDICE_1_LED_1);
                bitSet(ledState, UIMDICE_1_LED_2);
                bitSet(ledState, UIMDICE_1_LED_4);
                break;
            case (6):
                bitSet(ledState, UIMDICE_1_LED_1);
                bitSet(ledState, UIMDICE_1_LED_2);
                bitSet(ledState, UIMDICE_1_LED_3);
                break;
        }
    }
    // Dice 2
    else {
        switch (number) {
            case (1):
                bitSet(ledState, UIMDICE_2_LED_4);
                break;
            case (2):
                bitSet(ledState, UIMDICE_2_LED_1);
                break;
            case (3):
                bitSet(ledState, UIMDICE_2_LED_1);
                bitSet(ledState, UIMDICE_2_LED_4);
                break;
            case (4):
                bitSet(ledState, UIMDICE_2_LED_1);
                bitSet(ledState, UIMDICE_2_LED_2);
                break;
            case (5):
                bitSet(ledState, UIMDICE_2_LED_1);
                bitSet(ledState, UIMDICE_2_LED_2);
                bitSet(ledState, UIMDICE_2_LED_4);
                break;
            case (6):
                bitSet(ledState, UIMDICE_2_LED_1);
                bitSet(ledState, UIMDICE_2_LED_2);
                bitSet(ledState, UIMDICE_2_LED_3);
                break;
        }
    }
    updateShiftRegister();
}
