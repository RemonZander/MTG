#include "../../main/PathFinding.hpp"
#include "../../main/typedefs.h"
#include "../../main/Pawns.hpp"
#include "../../main/ludoStates.hpp"

#include <stdio.h>
#include <stddef.h>
#include <vector>

int main(void)
{
    PathFinding_impoved *pathFind = new PathFinding_impoved();

    BoardMap_t wightMap = {
        .size = {.x = BOARD_SIZE_X_LUDO, .y = BOARD_SIZE_Y_LUDO},
        .map = {
            {255, 255, 255, 255, 255,  10,   2,   2,   2,  10, 255, 255, 255, 255, 255},
            {255, 255,   1, 255,  20,  10,   2,  35,   2,  10,  20, 255,   1, 255, 255},
            {255,   1,   1,   1,  20,  10,   2,  35,   2,  10,  20,   1,   1,   1, 255},
            {255, 255,   1, 255,  20,  10,   2,  35,   2,  10,  20, 255,   1, 255, 255},
            {255,  20,  20,  20,  20,  10,   2,  35,   2,  10,  20,  20,  20,  20, 255},
            { 10,  10,  10,  10,  10,  10,   2,  35,   2,  10,  10,  10,  10,  10,  10},
            {  2,   2,   2,   2,   2,   2,   2, 255,   2,   2,   2,   2,   2,   2,   2},
            {  2,  35,  35,  35,  35,  35, 255, 255, 255,  35,  35,  35,  35,  35,   2},
            {  2,   2,   2,   2,   2,   2,   2, 255,   2,   2,   2,   2,   2,   2,   2},
            { 10,  10,  10,  10,  10,  10,   2,  35,   2,  10,  10,  10,  10,  10,  10},
            {255,  20,  20,  20,  20,  10,   2,  35,   2,  10,  20,  20,  20,  20, 255},
            {255, 255,   1, 255,  20,  10,   2,  35,   2,  10,  20, 255,   1, 255, 255},
            {255,   1,   1,   1,  20,  10,   2,  35,   2,  10,  20,   1,   1,   1, 255},
            {255, 255,   1, 255,  20,  10,   2,  35,   2,  10,  20, 255,   1, 255, 255},
            {255, 255, 255, 255, 255,  10,   2,   2,   2,  10, 255, 255, 255, 255, 255}
        }
    };


    Cordinates_s start = { .x = 0, .y = 6 };
    Cordinates_s end = { .x = 8, .y = 2 };
    std::vector<Pawn<LudoPawnState_t>*>* pawns = new std::vector<Pawn<LudoPawnState_t>*>{
        new Pawn<LudoPawnState_t>(0, Coordinates_t{2, 6}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{2, 6}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{0, 8}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{0, 8}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{6, 4}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{6, 4}})
    };

    pathfinding_path_t path = pathFind->findPath(start, end, wightMap, pawns, 255);
    delete(path);
}

