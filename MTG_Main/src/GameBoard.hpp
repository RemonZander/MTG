#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include "./MoterController.hpp"
#include "pawn.hpp"
class PathFinding;
class GameBoard
{
public:
    Pawn** pawns;
    int pawnAmount;
    int ratioX;
    int ratioY;
    PathFinding *path;
    int boardHeigth, boardWidth, originOffsetX, originOffsetY;

    GameBoard(int width, int height, int originOffsetX = 0, int originOffsetY = 0);
    ~GameBoard();
    bool movePawn(int fromX, int fromY, int toX, int toY);
    bool movePawn(Pawn *pawn, int toX, int toY);

    void addPawns(Pawn *pawns, int pawnAmount);


private:
    void printMap(int *map);
    bool rangeIncrement(int x, int y, int index, int *map);
    MoterController controller;
};

class PathFinding
{
private:
    byte *map;

public:
    PathFinding(GameBoard *board);
    GameBoard *board;
    ~PathFinding();

    bool find(int fromX, int fromY, int toX, int toY, int **instructions, int *arraySize);
    bool rangeIncrement(int x, int y, int value);
    void printMap();
    void updateMap();
    void displayPath(int *instructions, int arraySize);

private:
    void clearMap();
    int floodfill(int fromX, int fromY, int toX, int toY);
    void findPath(int *instructions, int size, int locX, int locY);
    void findLocOfNumber(byte number, int *locX, int *LoxY);
};

template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

#endif