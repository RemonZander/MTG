#pragma once
#include <GameBase.hpp>
#include <LudoUserInput.hpp>
#include <typedefs.h>
#include <ludoStates.hpp>

class LudoGame : public GameBase<LudoPlayerState_s, LudoPawnState_s, LudoGameState_t> {
    public:
        void GameLoop();
        void Init();

    private:
        uint8_t currentPlayer;
};