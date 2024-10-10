#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "config.h"

#include <cstdint>
#include <stdint.h>
#include <stdbool.h>
#include <vector>

typedef struct PF_Cords_s {
    uint8_t x;
    uint8_t y;
} PF_Cords_t;

typedef struct PF_Map_s {
    PF_Cords_t size;
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

typedef struct PF_ffQue_item_s {
    PF_Cords_t field;
    uint8_t pathWight;
} PF_ffQue_item_t;

class PathFinding_impoved
{
public:
    PathFinding_impoved();
    ~PathFinding_impoved();

    PF_Path_t findPath(PF_Cords_t start, PF_Cords_t end, PF_Map_t wightMap, const std::vector<PF_Cords_t> *powns, uint8_t pown_wight);

private:
    PF_Map_t wightMap;
    std::vector<PF_ffQue_item_t>* floodFill_Que;

    PF_Map_t floodFill(PF_Cords_t start, PF_Cords_t end);
    void floodFill_Que_add(PF_Cords_t field, uint8_t pathWight);
    PF_Cords_t floodFill_Que_pop();
    bool floodFill_scanField(PF_Cords_t field, PF_Map_t *result);
    PF_Path_t gatherPath(PF_Map_t floodFill_Map);
};

#endif
