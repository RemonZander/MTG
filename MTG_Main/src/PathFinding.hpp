#ifndef PATHFINDING_H
#define PATHFINDING_H

//#include <cstdint>
#include <stdint.h>
#include <stdbool.h>
#include <vector>

#include "config.h"
#include "typedefs.h"

typedef struct PF_Move_s {
    Coordinates_t start;
    Coordinates_t end;
} PF_Move_t;

typedef struct PF_Path_s {
    uint16_t moveNum;
    uint16_t totalPathLength;
    std::vector<PF_Move_t>* moves;
} PF_Path_t;

typedef struct PF_ffQue_item_s {
    Coordinates_t field;
    uint8_t pathWight;
} PF_ffQue_item_t;

class PathFinding_impoved
{
public:
    PathFinding_impoved();
    ~PathFinding_impoved();

    pathfinding_path_t findPath(Coordinates_t start, Coordinates_t end, BoardMap_t wightMap, const std::vector<Coordinates_t> *powns, uint8_t pown_wight);

private:
    BoardMap_t wightMap;
    std::vector<PF_ffQue_item_t>* floodFill_Que;

    BoardMap_t floodFill(Coordinates_t start, Coordinates_t end);
    void floodFill_Que_add(Coordinates_t field, uint8_t pathWight);
    Coordinates_t floodFill_Que_pop();
    bool floodFill_scanField(Coordinates_t field, BoardMap_t *result);
    pathfinding_path_t gatherPath(BoardMap_t floodFill_Map);
};

#endif
