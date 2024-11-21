#pragma once
#include <LudoUserInput.hpp>
#include <stdint.h>
#include <vector>

typedef struct LudoPlayerState_s {
    // LudoInputModule userInputModule;

    LudoInputModule userInputModule;
    bool HasPawnOnboard;

} LudoPlayerState_t;

typedef struct LudoPawnState_s {
    bool IsSelected;
    uint8_t Steps;
    bool IsHome;
    bool HasFinished;
    uint8_t CurrentGamePath;

} LudoPawnState_t;

typedef struct LudoGameState_s {
    LudoGameStates State;
    std::vector<Coordinates_t> GamePath;
} LudoGameState_t;