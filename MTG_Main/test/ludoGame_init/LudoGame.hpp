#pragma once
#include "./GameBase.hpp"
#include "./LudoUserInput.hpp"
#include "../../main/typedefs.h"
#include "./ludoStates.hpp"
#include "./ludoPlayer.hpp"

class LudoGame : public GameBase<LudoPlayerState_s, LudoPawnState_t, LudoGameState_t> {
    public:
        void GameLoop();
        void Init();

    private:
        uint8_t currentPlayer;
        void SaveGame();
        void RestoreGame();
};