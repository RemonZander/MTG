#pragma once
#include <Pawns.hpp>
#include <vector>
#include <Arduino.h>

template <class T, class U>
class Player {
    public:
        Player(uint8_t ID, std::vector<Pawn<U>*>* Pawns,T State) : ID(ID), State(State), Pawns(Pawns) {}

        uint8_t ID;
        std::vector<Pawn<U>*>* Pawns;
        T State;

        virtual void DoTurn();
};