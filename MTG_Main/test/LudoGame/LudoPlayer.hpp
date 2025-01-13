#pragma once
#include "../../main/Player.hpp"
#include "../../main/logger.h"
#include "./random.hpp"

using Random = effolkronium::random_static;

template <class T, class U>
class LudoPlayer : public Player<T, U> {
    public:
        LudoPlayer(uint8_t ID, std::vector<Pawn<U>*>* Pawns, T State) : Player<T,U>(ID, Pawns, State) {}

        void DoTurn()
        {
            LOG_I("Player %u turn has started", this->ID);
            uint8_t diceroll = Random::get<uint8_t>(1, 6) + Random::get<uint8_t>(1, 6);
            LOG_I("Player %u rolls: %u", this->ID, diceroll);

            int selectedPawn = -1;

            //can player do anything. if not return
            if (diceroll != 6 && !this->State.HasPawnOnboard) return;

            //can player do anything. if not return
            if (diceroll != 6 && ((*this->Pawns)[0]->State.HasFinished || (*this->Pawns)[0]->State.IsAtStart) && 
            ((*this->Pawns)[1]->State.HasFinished || (*this->Pawns)[1]->State.IsAtStart) &&
            ((*this->Pawns)[2]->State.HasFinished || (*this->Pawns)[2]->State.IsAtStart) &&
            ((*this->Pawns)[3]->State.HasFinished || (*this->Pawns)[3]->State.IsAtStart)) return;

            //The player can do a move. Let the player choose until the player has chosen a correct pawn
            do 
            {
                selectedPawn = Random::get<uint8_t>(0, 3);
                LOG_I("Player %u has selected pawn: %u", this->ID, selectedPawn);
                LOG_I("Pawn %u is on board: %u", selectedPawn, !(*this->Pawns)[selectedPawn]->State.IsAtStart);
                LOG_I("Pawn %u is in home lane: %u", selectedPawn, (*this->Pawns)[selectedPawn]->State.IsInHome);
                LOG_I("Pawn %u is finished: %u", selectedPawn, (*this->Pawns)[selectedPawn]->State.HasFinished);
            }
            while (((*this->Pawns)[selectedPawn]->State.IsAtStart && diceroll != 6) || (*this->Pawns)[selectedPawn]->State.HasFinished);
            
            LOG_I("Pawn %u has been selected. Returning back to gameloop", selectedPawn);
            (*this->Pawns)[selectedPawn]->State.IsSelected = true;
            (*this->Pawns)[selectedPawn]->State.Steps = diceroll;
        }
};