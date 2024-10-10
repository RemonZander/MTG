#include "../../src/PathFinding.hpp"

#include <stdio.h>
#include <stddef.h>
#include <vector>

int main(void)
{
    PathFinding_impoved pathFind = PathFinding_impoved();

    PF_Map_t wightMap = {
        .size = {.x = 10, .y = 10},
        .map = {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
        }
    };
    PF_Cords_t start = { .x = 2, .y = 2 };
    PF_Cords_t end = { .x = 7, .y = 7 };
    std::vector<PF_Cords_t> *powns = new std::vector<PF_Cords_t>;
    powns->push_back({.x = 1, .y = 7});
    powns->push_back({.x = 2, .y = 6});
    powns->push_back({.x = 3, .y = 5});
    powns->push_back({.x = 4, .y = 4});
    powns->push_back({.x = 5, .y = 3});
    // powns->push_back({.x = 6, .y = 2});
    powns->push_back({.x = 7, .y = 1});

    PF_Path_t path = pathFind.findPath(start, end, wightMap, powns, 5);
    delete(path.moves);
}

