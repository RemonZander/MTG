#ifndef LUDOGAME_HPP
#define LUDOGAME_HPP
#include "Player.hpp"
#include "LudoUserInput.hpp"
#include "LudoBoard.hpp"

class LudoGame{
    public:
        LudoGame(int playerAmount);
        int playerAmount;
        int currentPlayer = 0;
        Player *players[4];
        LudoInputModule UIMs[4] = {
            LudoInputModule(0x08),
            LudoInputModule(0x0B),
            LudoInputModule(0x0A),
            LudoInputModule(0x09)
        };
        ~LudoGame(){
            Serial.println("LudoGame, I say good day");
        }
        
        bool nextTurn(); //return false if game is done
        LudoBoard *board; 
};
#endif