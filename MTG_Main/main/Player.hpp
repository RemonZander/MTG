#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "pawn.hpp"
#include "logger.h"
#define PAWN_AMOUNT 4

class Player{
    public:
        Pawn *pawns[4];
        int playerNumber;
        int finish = 0;
        Player(){}
        Player(int playerNumber);

        ~Player(){
            println("Player, I say good day");
        }
};

#endif