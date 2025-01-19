#pragma once
#include <stdint.h>
#include "Player.hpp"
#include "MotionController.hpp"
#include "PathFinding.hpp"
#include <stdio.h>
#include <inttypes.h>

template <class T, class U, class V>
class GameBase {
    public:
        virtual void GameLoop() = 0;
        virtual void Init() = 0;
    
    protected:
        std::vector<Player<T, U>*>* players;
        BoardMap_t map;
        V state;
        MotionController *motion;
        PathFinding_impoved *Pathfinding;
};