#include "../../src/PathFinding.hpp"

#include <stdio.h>
#include <stddef.h>

int main(void)
{
    PathFinding_impoved pathFind = PathFinding_impoved();

    PF_Map_t wightMap = { .mapSize = {.x = 10, .y = 10}};
    PF_Cords_t start = { .x = 2, .y = 2 };
    PF_Cords_t end = { .x = 7, .y = 7 };

    PF_Path_t path = pathFind.findPath(start, end, wightMap, NULL);
}

