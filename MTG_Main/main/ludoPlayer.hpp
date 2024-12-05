#pragma once
#include <Player.hpp>

template <class T, class U>
class LudoPlayer : public Player<T, U> {
    public:
        LudoPlayer(uint8_t ID, std::vector<Pawn<U>*>* Pawns, T State) : Player<T,U>(ID, Pawns, State) {}

        void DoTurn()
        {
            Serial.println("Player " + String(this->ID) + " turn has started");
            uint8_t diceroll = this->State.userInputModule.rollDice();

            Serial.println("Player " + String(this->ID) + " rolls: " + String(diceroll));

            uint8_t selectedPawn = this->State.userInputModule.selectPawn();

            Serial.println("Player " + String(this->ID) + " has selected pawn: " + String(selectedPawn));
            
            if (this->State.HasPawnOnboard || diceroll == 6) 
            {
                (*this->Pawns)[selectedPawn]->State.IsSelected = true;
                (*this->Pawns)[selectedPawn]->State.Steps = diceroll;
            }
        }
};