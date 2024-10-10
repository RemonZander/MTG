#include "PathFinding.hpp"
#include "config.h"

#include <cstdint>
#include <cstring>
#include <stdio.h>
#include <vector>

#ifdef DEBUG_EXPORT
void print_map(PF_Map_t ffMap, char *indent)
{
    printf("{\n");
    printf("%s  \"size\": {\"x\": %u, \"y\": %u},\n", indent, ffMap.size.x, ffMap.size.y);
    printf("%s  \"map\": [\n", indent);
    for (int x=0; x<ffMap.size.x; x++)
    {
        printf("%s    [", indent);
        for (int y=0; y<ffMap.size.y; y++)
        {
            if (y == ffMap.size.y-1)
            {
                printf("%u", ffMap.map[x][y]);
            }
            else
            {
                printf("%u, ", ffMap.map[x][y]);
            }
        }
        if (x == ffMap.size.x-1)
        {
            printf("]\n");
        }
        else
        {
            printf("],\n");
        }
    }
    printf("%s  ]\n%s}", indent, indent);
}

void printQue(const std::vector<PF_ffQue_item_t>* que)
{
    printf("ffQue: [\n");
    for (int i = 0; i < que->size(); i++)
    {
        PF_ffQue_item_t item = que->at(i);
        printf("  {.wight = %u, .field = (%u, %u)},\n", item.pathWight, item.field.x, item.field.y);
    }
    printf("]\n");
}
#endif

enum PF_map_wights {
    EMPTY_FIELD = 253,
    TARGET_FIELD = 0,
    START_FIELD = 254,
    OPSTRUCTION_FIELD = 255
};

PathFinding_impoved::PathFinding_impoved()
{
    // do nothing??
}

PathFinding_impoved::~PathFinding_impoved()
{
    // do nothing??
}

PF_Path_t PathFinding_impoved::findPath(PF_Cords_t start, PF_Cords_t end, PF_Map_t wightMap, const std::vector<PF_Cords_t> *powns, uint8_t pown_wight)
{
    #ifdef DEBUG_EXPORT
    printf("{\n");
    printf("  \"inputs\": {\n");
    printf("    \"start\": {\"x\": %u, \"y\": %u},\n", start.x, start.y);
    printf("    \"end\": {\"x\": %u, \"y\": %u},\n", end.x, end.y);
    printf("    \"wightMap\": ");
    print_map(wightMap, "    ");
    printf("\n");
    printf("  },\n");
    printf("  \"steps\": [\n");
    #endif
    // if (fromX < 0 || fromX >= board->boardWidth || fromY < 0 || fromY >= board->boardHeigth)
    //     return false;
    // if (toX < 0 || toX >= board->boardWidth || toY < 0 || toY >= board->boardHeigth)
    //     return false;

    this->wightMap = wightMap;
    for (int i=0; i<powns->size(); i++)
    {
        const PF_Cords_t pown = powns->at(i);
        this->wightMap.map[pown.x][pown.y] = pown_wight;
    }

    PF_Map_t ffMap = floodFill(start, end);

    #ifdef DEBUG_EXPORT
    printf("  ],\n");
    printf("  \"result_floodFill_map\": ");
    print_map(ffMap, "  ");
    printf(",\n");
    #endif

    PF_Path_t path = gatherPath(ffMap);

    #ifdef DEBUG_EXPORT
    printf("  \"result_path\": {\n");
    printf("    \"moveNum\": %i,\n", path.moveNum);
    printf("    \"totalPathLength\": %u,\n", path.totalPathLength);
    printf("    \"moves\": [\n");
    for (int i = 0; i < path.moves->size(); i++)
    {
        PF_Move_t move = path.moves->at(i);
        printf("      {\n");
        printf("        \"start\": [%u, %u],\n", move.start.x, move.start.y);
        printf("        \"end\": [%u, %u]\n", move.end.x, move.end.y);
        if (i == path.moves->size()-1)
        {
            printf("      }\n");
        }
        else
        {
            printf("      },\n");
        }
    }
    printf("    ]\n  }\n}\n");
    #endif

    //TODO: check for powns to move

    return path;
}

PF_Map_t PathFinding_impoved::floodFill(PF_Cords_t start, PF_Cords_t end)
{
    bool pathFound = false;
    int index = TARGET_FIELD;
    PF_Map_t result;

    // inti the result map
    memset(&result, EMPTY_FIELD, sizeof(PF_Map_t));

    result.size.x = wightMap.size.x;
    result.size.y = wightMap.size.y;

    result.map[end.x][end.y] = TARGET_FIELD;
    result.map[start.x][start.y] = START_FIELD;

    floodFill_Que = new std::vector<PF_ffQue_item_t>;
    floodFill_Que->push_back({ .field = end, .pathWight = 0});

    while (!pathFound && index < PF_WIEGHT_MAX)
    {
        PF_Cords_t curField = floodFill_Que_pop();

        if (curField.x != 255 || curField.y != 255)
        {
            pathFound = floodFill_scanField(curField, &result);
        }
        else
        { // que is empty, becouse there is no path posible
            // memset(&result, EMPTY_FIELD, sizeof(PF_Map_t));
            return result;
        }

        #ifdef DEBUG_EXPORT
        printf("    ");
        print_map(result, "    ");
        if (pathFound)
        {
            printf("\n");
        }
        else
        {
            printf(",\n");
        }
        #endif
        index++;
    }

    return result;
}

void PathFinding_impoved::floodFill_Que_add(PF_Cords_t field, uint8_t pathWight)
{
    const PF_ffQue_item_t newQueItem = { .field = field, .pathWight = pathWight};
    bool inserted = false;
    for (std::vector<PF_ffQue_item_t>::iterator it = floodFill_Que->begin(); it != floodFill_Que->end() && !inserted; it++)
    {
        if (it->pathWight > pathWight)
        {
            floodFill_Que->insert(it, newQueItem);
            inserted = true;
        }
    }
    if (!inserted)
    {
        if (floodFill_Que->end()->pathWight > pathWight)
        {
            floodFill_Que->insert(floodFill_Que->end(), newQueItem);
        }
        else
        {
            floodFill_Que->push_back(newQueItem);
        }
    }
}

PF_Cords_t PathFinding_impoved::floodFill_Que_pop()
{
    PF_Cords_t result = {.x = 255, .y = 255};
    if (floodFill_Que->size() != 0)
    {
        result = floodFill_Que->at(0).field;
        floodFill_Que->erase(floodFill_Que->begin());
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
        const PF_Cords_t curField = {
            .x = (uint8_t)(field.x + cordModifiers[i][0]),
            .y = (uint8_t)(field.y + cordModifiers[i][1])
        };

        if (curField.y < 0 || curField.y >= result->size.y || curField.x < 0 || curField.x >= result->size.x)
            continue;

        switch (result->map[curField.x][curField.y])
        {
            case EMPTY_FIELD:
                result->map[curField.x][curField.y] = result->map[field.x][field.y] + this->wightMap.map[curField.x][curField.y];
                floodFill_Que_add(curField, result->map[curField.x][curField.y]);
                break;
            case START_FIELD:
                targetFound = true;
                break;
            case TARGET_FIELD:
                break;
            case OPSTRUCTION_FIELD:
                break;
            default:
                break;
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
    result.moves = new std::vector<PF_Move_t>;

    // find start and end positions
    PF_Cords_t curField;
    for (curField.x = 0; curField.x < floodFill_Map.size.x; curField.x++)
    {
        for (curField.y = 0; curField.y < floodFill_Map.size.y; curField.y++)
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

    curPos = startPos;

    for(int index = 0; index < PF_PATH_MOVE_LEN_MAX; index++)
    {
        uint8_t minWieght_val = 255;
        PF_Cords_t minWieght_pos;

        for (int i = 0; i < 4; i++)
        {
            int16_t x = curPos.x + cordModifiers[i][0];
            int16_t y = curPos.y + cordModifiers[i][1];
    
            if (y < 0 || y >= floodFill_Map.size.y || x < 0 || x >= floodFill_Map.size.x)
                continue;

            if (floodFill_Map.map[x][y] < minWieght_val)
            {
                minWieght_val = floodFill_Map.map[x][y];
                minWieght_pos.x = x;
                minWieght_pos.y = y;
            }
        }

        result.moveNum++;
        result.totalPathLength++;
        result.moves->push_back({
            .start = minWieght_pos,
            .end = curPos
        });
        curPos = minWieght_pos;
        if (minWieght_val == TARGET_FIELD)
        {
            break;
        }
    }

    return result;
}
