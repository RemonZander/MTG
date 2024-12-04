#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "pawn.hpp"
#include <arduino.h>
#define PAWN_AMOUNT 4

class Player{
    public:
        Pawn *pawns[4];
        int playerNumber;
        int finish = 0;
        Player(){}
        Player(int playerNumber);

                ~Player(){
            Serial.println("Player, I say good day");
        }
};

#endif