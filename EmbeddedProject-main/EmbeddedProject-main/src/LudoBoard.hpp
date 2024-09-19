#ifndef LUDOBOARD_HPP
#define LUDOBOARD_HPP
#include "GameBoard.hpp"
#include "pawn.hpp"

class LudoBoard: public GameBoard {
    public:
        LudoBoard(int w, int h, int offX, int offY) : GameBoard(w, h, offX, offY){}
        void movePawn(Pawn* pawn, int steps);
        void movePawn(Pawn* pawn, int x, int y) {GameBoard::movePawn(pawn, x, y);}

        ~LudoBoard(){
            Serial.println("LudoBoard, I say good day");
        }

    private:
        int stepToXPos(int step);
        int stepToYPos(int step);
        void capturePawn(int x, int y);
        const double FREQ = 13;
};

#endif