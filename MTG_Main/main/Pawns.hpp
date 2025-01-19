#pragma once
#include "typedefs.h"

template <class T>
class Pawn {
    public:
        Pawn (uint8_t ID, Coordinates_t squareCords, T State){
            this->ID = ID;
            this->squareCords = squareCords;
            this->State = State;
        }
        uint8_t ID;
        Coordinates_t squareCords;
        T State;
};