#include "../../src/PathFinding.hpp"
#include "../../src/typedefs.h"

#include <stdio.h>
#include <stddef.h>
#include <vector>

int main(void)
{
    PathFinding_impoved pathFind = PathFinding_impoved();

    BoardMap_t wightMap = {
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
    Cordinates_s start = { .x = 2, .y = 2 };
    Cordinates_s end = { .x = 7, .y = 7 };
    std::vector<Cordinates_s> *powns = new std::vector<Cordinates_s>;
    powns->push_back({.x = 1, .y = 7});
    powns->push_back({.x = 2, .y = 6});
    powns->push_back({.x = 3, .y = 5});
    // powns->push_back({.x = 4, .y = 4});
    powns->push_back({.x = 5, .y = 3});
    powns->push_back({.x = 6, .y = 2});
    powns->push_back({.x = 7, .y = 1});

    pathfinding_path_t path = pathFind.findPath(start, end, wightMap, powns, 5);
    delete(path);
}

