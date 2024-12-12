#pragma once
#include <Player.hpp>
#include "logger.h"

template <class T, class U>
class LudoPlayer : public Player<T, U> {
    public:
        LudoPlayer(uint8_t ID, std::vector<Pawn<U>*>* Pawns, T State) : Player<T,U>(ID, Pawns, State) {}

        void DoTurn()
        {
            LOG_I("Player %u turn has started", this->ID);
            uint8_t diceroll = this->State.userInputModule.rollDice();

            LOG_I("Player %u rolls: %u", this->ID, diceroll);

            uint8_t selectedPawn = this->State.userInputModule.selectPawn();

            LOG_I("Player %u has selected pawn: %u", this->ID, selectedPawn);
            
            if (this->State.HasPawnOnboard || diceroll == 6) 
            {
                (*this->Pawns)[selectedPawn]->State.IsSelected = true;
                (*this->Pawns)[selectedPawn]->State.Steps = diceroll;
            }
        }
};