#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <stdint.h>
#include <stdbool.h>
#include <vector>
#include "ludoStates.hpp"
#include "Pawns.hpp"

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

    /** findPath
     * generate a path for a pawn to take.
     * 
     * args:
     * - start: start coordiantes
     * - end: end coordiantes (destination)
     * - wightMap: a map with all the weights for each field
     * - pawns: a list of the locations of the pawns
     * - pown_wight: the weight used for pawns. this sould always be 255, 
     *               otherwise you can get paths that moves over other pawns
     */
    pathfinding_path_t findPath(Coordinates_t start, Coordinates_t end, 
        BoardMap_t wightMap, const std::vector<Pawn<LudoPawnState_t>*>* pawns, uint8_t pown_wight);

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
