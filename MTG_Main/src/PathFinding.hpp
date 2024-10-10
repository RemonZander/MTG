#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "config.h"

#include <stdint.h>
#include <stdbool.h>
#include <vector>

typedef struct PF_Cords_s {
    uint8_t x;
    uint8_t y;
} PF_Cords_t;

typedef struct PF_Map_s {
    PF_Cords_t mapSize;
    uint8_t map[BOARD_SIZE_X_MAX][BOARD_SIZE_Y_MAX];
} PF_Map_t;

typedef struct PF_Move_s {
    PF_Cords_t start;
    PF_Cords_t end;
} PF_Move_t;

typedef struct PF_Path_s {
    uint16_t moveNum;
    uint16_t totalPathLength;
    std::vector<PF_Move_t>* moves;
} PF_Path_t;

class PathFinding_impoved
{
public:
    PathFinding_impoved();
    ~PathFinding_impoved();

    PF_Path_t findPath(PF_Cords_t start, PF_Cords_t end, PF_Map_t wightMap, const PF_Cords_s *powns, uint8_t pown_wight);

private:
    PF_Map_t floodFill(PF_Cords_t start, PF_Cords_t end, PF_Map_t wightMap);
    bool floodFill_scanField(PF_Cords_t field, PF_Map_t *result);
    PF_Path_t gatherPath(PF_Map_t floodFill_Map);
};

#endif
