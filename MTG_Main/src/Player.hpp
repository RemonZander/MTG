#include <Pawns.hpp>
#include <vector>
#include <typedefs.h>
#include <Arduino.h>

template <class T, class U>
class Player {
    public:
        Player(uint8_t ID, std::vector<Pawn<U>*>* Pawns,T State) : ID(ID), State(State), Pawns(Pawns) {}

        uint8_t ID;
        std::vector<Pawn<U>*>* Pawns;
        T State;

        LudoGameStates DoTurn()
        {
            Serial.println("Player " + String(ID) + " does a move");
            int diceroll = State.rollDice();
            if (ID == 3) return LudoGameStates::Player1;
            return (LudoGameStates)(ID + 1);
        }
};