#include "UimSlave.hpp"
#include "ButtonManager.hpp"
#include "DiceService.hpp"
#include "UIMCommand.h"
#include "UIMButton.h"

#define UIM_ADDRESS 0xA

UimSlave communicationService;
ButtonManager buttonManager;
DiceService diceService;

int state;
int commandList;

long timerStartTime1;
int blinkNo = 0;
int thrownNumber;

// If the command is an acknowledge, remove all the commands from the list and reset the state.
// Else, add the command to the command list.
void parseCommand(int command) {
  if (command & UIMCOMMAND_ACKNOWLEDGE) {
    state = 0x0;
    commandList = 0x0;
    buttonManager.toggleLed(UIMBUTTON_LED_DICEROLL, 0);
    buttonManager.toggleLed(UIMBUTTON_LED_PAWNS, 0);
    diceService.resetDice();
  }
  else
    commandList |= command;
}

void setup() {
  communicationService.begin(UIM_ADDRESS, parseCommand, &state);
}

void loop() {
  // If the diceroll is in the command list, generate a random number. Only when the diceroll button is pressed, add the number thrown to the state and remove the dice thrown from the command list.
  if (commandList & UIMCOMMAND_ENABLE_DICE_1 || commandList & UIMCOMMAND_ENABLE_DICE_2) {
    buttonManager.toggleLed(UIMBUTTON_LED_DICEROLL, 1);
    if (millis() - timerStartTime1 >= 100) {
      thrownNumber = diceService.generateNumber(commandList & UIMCOMMAND_ENABLE_DICE_2);
      timerStartTime1 = millis();
    }
    if (buttonManager.buttonPressed() == UIMBUTTON_DICEROLL) {
      buttonManager.toggleLed(UIMBUTTON_LED_DICEROLL, 0);
      state |= thrownNumber;
      if (commandList & UIMCOMMAND_ENABLE_DICE_1)
        commandList ^= UIMCOMMAND_ENABLE_DICE_1;
      else
        commandList ^= UIMCOMMAND_ENABLE_DICE_2;
    }
  }

  // If the pawn selection is in the command list, check if one of the pawn selection buttons is pressed. If true, add the selected pawn to the state and remove the pawn selection from the command list.
  // Is an else if, because dice rolling has priority over pawn selection. (You always roll a dice before moving a pawn).
  else if (commandList & UIMCOMMAND_ENABLE_PAWN_SELECTION) {
    buttonManager.toggleLed(UIMBUTTON_LED_PAWNS, 1);
    uimButton buttonPressed = buttonManager.buttonPressed();
    if (buttonPressed >= UIMBUTTON_PAWN_1 && buttonPressed <= UIMBUTTON_PAWN_4) {
      (buttonPressed == UIMBUTTON_PAWN_1) ? state |= 0x0100 : (buttonPressed == UIMBUTTON_PAWN_2) ? state |= 0x0200 : (buttonPressed == UIMBUTTON_PAWN_3) ? state |= 0x0400 : state |= 0x0800;
      buttonManager.toggleLed(UIMBUTTON_LED_PAWNS, 0);
      commandList ^= UIMCOMMAND_ENABLE_PAWN_SELECTION;
    }
  }

  // Blink (for debug possibilites)
  if (commandList & UIMCOMMAND_BLINK) {
    if (millis() - timerStartTime1 < 500) {
      buttonManager.toggleLed(UIMBUTTON_LED_DICEROLL, 0);
      buttonManager.toggleLed(UIMBUTTON_LED_PAWNS, 1);
    }
    else if (millis() - timerStartTime1 >= 500 && millis() - timerStartTime1 < 1000) {
      buttonManager.toggleLed(UIMBUTTON_LED_DICEROLL, 1);
      buttonManager.toggleLed(UIMBUTTON_LED_PAWNS, 0);
    }
    else if (millis() - timerStartTime1 > 1000) {
      timerStartTime1 = millis();
      blinkNo++;
    }
    if (blinkNo > 5) {
      blinkNo = 0;
      buttonManager.toggleLed(UIMBUTTON_LED_DICEROLL, 0);
      buttonManager.toggleLed(UIMBUTTON_LED_PAWNS, 0);
      commandList ^= UIMCOMMAND_BLINK;
    }
  }
}
