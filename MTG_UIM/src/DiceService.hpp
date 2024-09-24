#ifndef DICESERVICE_H
#define DICESERVICE_H

#include "Arduino.h"

class DiceService {  
    public:
        DiceService();
        int generateNumber(bool twoDice);
        void resetDice();
    
    private:
        int lastThrown1;
        int lastThrown2;
        byte ledState;
        void updateShiftRegister();
        void showNumber(int diceNumber, int number);
};
#endif