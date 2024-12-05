#ifndef PAWN_HPP
#define PAWN_HPP

#include <Arduino.h>

class Pawn {  
    public:
        int x, y, HomeX, HomeY, id, step = 2, beginStep = 0; // step = 2 becouse 0 is the finish lane entry, and the starting position is 2 tiles further.
        int finishX, finishY;
        Pawn(){}
        Pawn(int x, int y, int id);
        bool isOnBoard = false;
        bool isFinished = false;
        ~Pawn(){
            Serial.println("Oh my goooodddd");
        }
        void FinishPawn();

};
#endif
