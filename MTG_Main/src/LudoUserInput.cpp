#include "LudoUserInput.hpp"
#include "arduino.h"

int LudoInputModule:: rollDice(){
    transmit(UIMCOMMAND_ENABLE_DICE_2);
    int diceroll = 0;
    do {
        int state = requestState();
        diceroll = state >> 8;
    } while (diceroll == 0);
    delay(1000);
    transmit(UIMCOMMAND_ACKNOWLEDGE);
    Serial.println(diceroll);
    return diceroll;
};

int LudoInputModule:: selectPawn(){
    transmit(UIMCOMMAND_ENABLE_PAWN_SELECTION);
    int selectedPawn = 0;
    do {
        int state = requestState();
        selectedPawn = state & 0xff;
        if (selectedPawn == 4) selectedPawn = 3;
        if (selectedPawn == 8) selectedPawn = 4;            /// maurice bedenk ff comment voor dit
    } while (selectedPawn == 0);
    selectedPawn -= 1;              // so we can use this as an index
    delay(100);
    transmit(UIMCOMMAND_ACKNOWLEDGE);
    Serial.println(selectedPawn);
    return selectedPawn;
};