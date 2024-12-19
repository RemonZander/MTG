#pragma once
#include "LudoUserInput.hpp"
#include <stdint.h>
#include <vector>
#include "typedefs.h"

typedef struct LudoPlayerState_s {
    // LudoInputModule userInputModule;

    LudoInputModule userInputModule;
    bool HasPawnOnboard;

} LudoPlayerState_t;

typedef struct LudoPawnState_s {
    bool IsSelected;
    uint8_t Steps;
    bool IsAtStart;
    bool IsInHome;
    bool HasFinished;
    Coordinates_t startPos;
    uint8_t CurrentGamePath;

} LudoPawnState_t;

typedef struct LudoGameState_s {
    LudoGameStates State;
    std::vector<Coordinates_t> GamePath;
    std::vector<std::vector<Coordinates_t>*> homePositions;
    std::vector<Pawn<LudoPawnState_t>*> allPawns;
} LudoGameState_t;