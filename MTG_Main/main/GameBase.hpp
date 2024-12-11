#pragma once
#include <stdint.h>
#include <Player.hpp>
#include <MotionController.hpp>
#include <PathFinding.hpp>

template <class T, class U, class V>
class GameBase {
    public:
        virtual void GameLoop();
        virtual void Init();
    
    protected:
        std::vector<Player<T, U>*>* players;
        BoardMap_t map;
        V state;
        MotionController *motion;
        PathFinding_impoved *Pathfinding;

};