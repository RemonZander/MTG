#pragma once
#include "../../main/Player.hpp"
#include "logger.h"
#include <random.hpp>

using Random = effolkronium::random_static;

template <class T, class U>
class LudoPlayer : public Player<T, U> {
    public:
        LudoPlayer(uint8_t ID, std::vector<Pawn<U>*>* Pawns, T State) : Player<T,U>(ID, Pawns, State) {}

        void DoTurn()
        {
            LOG_I("Player %u turn has started", this->ID);
            uint8_t diceroll = Random::get<uint8_t>(1, 6);

            LOG_I("Player %u rolls: %u", this->ID, diceroll);

            uint8_t selectedPawn = Random::get<uint8_t>(1, 3);

            LOG_I("Player %u has selected pawn: %u", this->ID, selectedPawn);
            
            while ((*this->Pawns)[selectedPawn]->State.IsAtStart && diceroll != 6)
            {
                selectedPawn = Random::get<uint8_t>(1, 3);
                LOG_I("Player %u has selected pawn: %u", this->ID, selectedPawn);
            }
            
            (*this->Pawns)[selectedPawn]->State.IsSelected = true;
            (*this->Pawns)[selectedPawn]->State.Steps = diceroll;
        }
};