#include "GameBoard.hpp"
#include "Memory.cpp"


#define DEST_DEF 255
#define PAWN_DEF 80
#define MAX_FLOOD_ITT 79

PathFinding::PathFinding(GameBoard *board) {
    this->board = board;
    int arrayLength = (board->boardWidth) * (board->boardHeigth);
    map = new byte[arrayLength];

    clearMap();
}

PathFinding::~PathFinding()
{
}

void PathFinding::updateMap()
{
    clearMap();


    for (int i = 0; i < board->pawnAmount; i++)
    {
        int pawnX = board->pawns[i]->x +board->originOffsetX;
        int pawnY = board->pawns[i]->y+board->originOffsetY;
       
        if(pawnX < 0 || pawnX >= board->boardWidth ||pawnY < 0 || pawnY >= board->boardHeigth)
            continue;
        
        map[pawnX * board->boardWidth + pawnY] = board->pawns[i]->id + PAWN_DEF;
    }
}
void PathFinding::clearMap()
{

    for (int x = 0; x < board->boardWidth; x++)
    {
        for (int y = 0; y < board->boardHeigth; y++){
            map[x * board->boardWidth + y] = 0;
        }
    }
}

void PathFinding::printMap()
{
    for (int x = 0; x < board->boardWidth; x++)
    {
        for (int y = 0; y < board->boardHeigth; y++)
        {
            Serial.print(map[x * board->boardWidth + y]);
            if (map[x * board->boardWidth + y] < 10)
                Serial.print(" ");
            Serial.print(" ");
        }
        Serial.println();
    }
}

void PathFinding::displayPath(int *instructions, int arraySize)
{
    clearMap();

    for (int i = 0; i < arraySize; i++)
    {
        byte x = instructions[i * 2] +board->originOffsetX ;
        byte y = instructions[i * 2 + 1]+board->originOffsetY;

        map[x * board->boardWidth + y] = i;
    }

    printMap();
}

bool PathFinding::find(int fromX, int fromY, int toX, int toY,int **instructions,int *arraySize)
{
    fromX +=board->originOffsetX;
    fromY += board->originOffsetY;
    toX += board->originOffsetX;
    toY += board->originOffsetY;

    if (fromX < 0 || fromX >= board->boardWidth || fromY < 0 || fromY >= board->boardHeigth)
        return false;
    if (toX < 0 || toX >= board->boardWidth || toY < 0 || toY >= board->boardHeigth)
        return false;

    clearMap();
    updateMap();

    int stepCount = floodfill(fromX, fromY, toX, toY);

    if (stepCount == -1)
    {
        Serial.println("Floodfailed");
        Serial.print(fromX);
        Serial.print("|");
        Serial.print(fromY);
        Serial.print("\t");
        Serial.print(toX);
        Serial.print("|");
        Serial.println(toY);

        return false;
    }

    *instructions = new int[(stepCount - 1) * 2];
    *arraySize = stepCount - 1;

    findPath(*instructions, stepCount, fromX, fromY);

    return true;
}

int PathFinding::floodfill(int fromX, int fromY, int toX, int toY)
{
    bool done = false;
    int index = 1;
    if (map[toX * board->boardWidth + toY] != 0)
        return -1; 

    map[toX * board->boardWidth + toY] = 1;
    map[fromX * board->boardWidth + fromY] = DEST_DEF;

    while (!done)
    { 
        for (int x = 0; x < board->boardWidth && !done; x++)
        {
            for (int y = 0; y < board->boardHeigth && !done; y++)
            {
                if (index == map[x * board->boardWidth + y])
                {
                    done = rangeIncrement(x, y,index);
                } 
            }
        }
        if(index == MAX_FLOOD_ITT)
                return -1; 

        index++;
    }

    return index;
}

/**
 * Sets the value around the (x,y) int the map if value at index is 0
 *
 * @return true if DEST_DEF is found
 */
bool PathFinding::rangeIncrement(int x, int y, int value)
{
    for (int i = 0; i < 4; i++)
    {
        int y1 = y + (i % 2 == 0 ? sgn(i - 1) : 0);
        int x1 = x + (i % 2 == 0 ? 0 : -sgn(i - 2));

        if (y1 < 0 || y1 >= board->boardHeigth || x1 < 0 || x1 >= board->boardWidth)
            continue;
            
        if (map[x1 * board->boardWidth + y1] == DEST_DEF)
        {
            return true;
        }

        if (map[x1 * board->boardWidth + y1] == 0)
            map[x1 * board->boardWidth + y1] = value + 1;
    }
    return false;
}

void PathFinding::findPath(int *instructions, int size, int locX, int locY)
{
    
    int currentSearchValue = size-1;
    int xTemp = locX;
    int yTemp = locY;

    for(int index = 0 ; index<size; index++){
        
        for (int i = 0; i < 4; i++)
        {
            int x1 = xTemp + (i % 2 == 0 ? 0 : -sgn(i - 2));
            int y1 = yTemp + (i % 2 == 0 ? sgn(i - 1) : 0);

            if (y1 < 0 || y1 >= board->boardHeigth || x1 < 0 || x1 >= board->boardWidth)
                continue;
      
            if(map[x1*board->boardWidth+y1] == (currentSearchValue)){
                xTemp = x1;
                yTemp = y1;
                break;
            }
        }

        instructions[index*2] = xTemp - board->originOffsetX;
        instructions[index*2+1] = yTemp - board->originOffsetY;

        map[xTemp*board->boardWidth+yTemp] = 255;
        Serial.print("Mem :");
        Serial.println(freeRAM());
        currentSearchValue--;
    }
   
}

void PathFinding::findLocOfNumber(byte number, int *locX, int *locY)
{
    for (int x = 0; x < board->boardWidth; x++)
    {
        for (int y = 0; y < board->boardHeigth; y++)
        {
            if (map[x * board->boardWidth + y] == number)
            {
                *locX = x;
                *locY = y;
            }
        }
    }
}
