#pragma once
#include <LudoUserInput.hpp>
#include <stdint.h>

typedef struct LudoPlayerState_s {
    // LudoInputModule userInputModule;

    LudoInputModule userInputModule;
    bool HasPawnOnboard;


} LudoPlayerState_t;

typedef struct LudoPawnState_s {
    bool IsSelected;
    uint8_t Steps;

} LudoPawnState_t;