#ifndef PAWN_HPP
#define PAWN_HPP

#include "logger.h"

class Pawn {  
    public:
        int x;
        int y;
        int id;
        int HomeX;
        int HomeY;
        int step = 2; // step = 2 becouse 0 is the finish lane entry, and the starting position is 2 tiles further.
        int beginStep = 0;
        int finishX;
        int finishY;
        Pawn(){}
        Pawn(int x, int y, int id);
        bool isOnBoard = false;
        bool isFinished = false;
        ~Pawn(){
            println("Oh my goooodddd");
        }
        void FinishPawn();

};
#endif
