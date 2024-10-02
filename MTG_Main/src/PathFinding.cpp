#include "PathFinding.hpp"
#include "config.h"

#include <cstdint>
#include <cstring>
#include <stdio.h>

enum PF_map_wights {
    EMPTY_FIELD = 0,
    TARGET_FIELD = 255,
    START_FIELD = 254
};

PathFinding_impoved::PathFinding_impoved()
{
    // do nothing??
}

PathFinding_impoved::~PathFinding_impoved()
{
    // do nothing??
}

PF_Path_t PathFinding_impoved::findPath(PF_Cords_t start, PF_Cords_t end, PF_Map_t wightMap, const PF_Cords_s *powns)
{
    // if (fromX < 0 || fromX >= board->boardWidth || fromY < 0 || fromY >= board->boardHeigth)
    //     return false;
    // if (toX < 0 || toX >= board->boardWidth || toY < 0 || toY >= board->boardHeigth)
    //     return false;

    //TODO: add powns to wightMap

    PF_Map_t ffMap = floodFill(start, end, wightMap);

    PF_Path_t path = gatherPath(ffMap);

    //TODO: check for powns to move

    return path;
}

PF_Map_t PathFinding_impoved::floodFill(PF_Cords_t start, PF_Cords_t end, PF_Map_t wightMap)
{
    bool pathFound = false;
    int index = 1;
    PF_Map_t result;

    // inti the result map
    memset(&result, EMPTY_FIELD, sizeof(PF_Map_t));

    result.map[end.x][end.y] = TARGET_FIELD;
    result.map[start.x][start.y] = START_FIELD;

    while (!pathFound && index < PF_WIEGHT_MAX)
    { 
        PF_Cords_t curField = { .x = 0, .y = 0 };

        for (; curField.x < wightMap.mapSize.x && !pathFound; curField.x++)
        {
            for (; curField.y < wightMap.mapSize.y && !pathFound; curField.y++)
            {
                if (result.map[curField.x][curField.y] == index)
                {
                    pathFound = floodFill_scanField(curField, &result);
                } 
            }
        }

        index++;
    }

    return result;
}

/**
 * Sets the value around the (x,y) int the map if value at index is 0
 *
 * @return true if DEST_DEF is found
 */
bool PathFinding_impoved::floodFill_scanField(PF_Cords_t field, PF_Map_t *result)
{
    const int8_t cordModifiers[4][2] = {
        { 1,  0},
        { 0,  1},
        {-1,  0},
        { 0, -1}
    };
    bool targetFound = false;
    for (int i = 0; i < 4; i++)
    {
        int16_t x = field.x + cordModifiers[i][0];
        int16_t y = field.y + cordModifiers[i][1];

        if (y < 0 || y >= result->mapSize.y || x < 0 || x >= result->mapSize.x)
            continue;
            
        switch (result->map[x][y]) {
            case EMPTY_FIELD:
                result->map[x][y] = result->map[field.x][field.y] + 1;
                break;
            case START_FIELD:
                break;
            case TARGET_FIELD:
                targetFound = true;
            default:
                break;
        }

        if (targetFound)
        {
            return true;
        }
    }
    return targetFound;
}

PF_Path_t PathFinding_impoved::gatherPath(PF_Map_t floodFill_Map)
{
    const int8_t cordModifiers[4][2] = {
        { 1,  0},
        { 0,  1},
        {-1,  0},
        { 0, -1}
    };

    PF_Cords_t startPos, endPos, curPos;
    PF_Path_t result;
    memset(&result, 0, sizeof(PF_Path_t));

    // find start and end positions
    for (PF_Cords_t curField = {.x = 0, .y = 0}; curField.x < floodFill_Map.mapSize.x; curField.x++)
    {
        for (; curField.y < floodFill_Map.mapSize.y; curField.y++)
        {
            switch (floodFill_Map.map[curField.x][curField.y]) {
                case START_FIELD:
                    startPos = curField;
                    break;
                case TARGET_FIELD:
                    endPos = curField;
                    break;
                default:
                    break;
            }
        }
    }

    curPos = endPos;

    for(int index = 0; index < PF_PATH_MOVE_LEN_MAX; index++)
    {
        uint8_t minWieght_val = 255;
        PF_Cords_t minWieght_pos;

        for (int i = 0; i < 4; i++)
        {
            int16_t x = curPos.x + cordModifiers[i][0];
            int16_t y = curPos.y + cordModifiers[i][1];

            if (y < 0 || y >= floodFill_Map.mapSize.y || x < 0 || x >= floodFill_Map.mapSize.x)
                continue;

            if (floodFill_Map.map[x][y] < minWieght_val)
            {
                minWieght_val = floodFill_Map.map[x][y];
                minWieght_pos = curPos;
            }
        }

        result.moveNum++;
        result.totalPathLength++;
        result.moves[index] = {
            .start = minWieght_pos,
            .end = curPos
        };
    }

    return result;
}
