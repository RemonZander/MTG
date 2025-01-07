#pragma once

#ifdef ARDUONO
#include <Arduino.h>
#endif

#include "Pawns.hpp"
#include <vector>

template <class T, class U>
class Player {
    public:
        Player(uint8_t ID, std::vector<Pawn<U>*>* Pawns,T State) : ID(ID), State(State), Pawns(Pawns) {}

        uint8_t ID;
        T State;
        std::vector<Pawn<U>*>* Pawns;

        virtual void DoTurn() = 0;
};