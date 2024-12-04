#include "logger.h"
#include "GameBoard.hpp"
#include <math.h>

GameBoard::GameBoard(int width, int height, int originOffsetX, int originOffsetY) : boardHeigth(height), boardWidth(width), originOffsetX(originOffsetX), originOffsetY(originOffsetY)
{
    println("GameBoard init");
    this->controller = MotionController();
    // this->controller.HomeMotors();

    // ratioX = this->controller.getMaxBoardX() / (width-1);
    // ratioY = this->controller.getMaxBoardY() / (height-1);

    //cursed line. Circulair references. Lot of shared states
    path = new PathFinding(this);
}

GameBoard::~GameBoard()
{
    println("GameBoard, I say good day");

    if(path != nullptr)
        delete(path);
}


bool GameBoard::movePawn(int fromX, int fromY, int toX, int toY)
{
    /*
   println("Moving to");
   print(toX);
   print("|");
   print(toY);
    */


    fromX += originOffsetX;
    fromY += originOffsetY;
    toX += originOffsetX;
    toY += originOffsetY;

    if ((fromX > boardWidth-1 || fromY > boardHeigth-1) || (toX > boardWidth-1 || toY > boardHeigth-1))
    {
        println("Oopsie woospie ik mag daar niet naartoe");
        // print(fromX);
        // print(" | ");
        // print(fromY);
        // print(" | ");
        // print(toX);
        // print(" | ");
        // println(fromY);
        return false;
    }
    if ((fromX < 0 || fromY < 0) || (toX < 0 || toY < 0))
    {
        println("Oopsie woospie dat is te negatief ");
        // print(fromX);
        // print(" | ");
        // print(fromY);
        // print(" | ");
        // print(toX);
        // print(" | ");
        // println(fromY);
        return false;
    }

   //println("test");

    int sourceX = (fromX * ratioX);
    int sourceY = (fromY * ratioY);

    int destX = (toX * ratioX);
    int destY = (toY * ratioY);


    // this->controller.SetMagnet(false);
    // this->controller.MotorToPos(sourceX, sourceY);
    // this->controller.SetMagnet(true);
    // this->controller.MotorToPos(destX, destY);
    
    //delay(500);
    this->controller.SetMagnet(false);
    return true;
}

bool GameBoard::movePawn(Pawn *pawn, int toX, int toY)
{

    bool output = movePawn(pawn->x, pawn->y, toX, toY);

    if (output == false)
        return false;

    pawn->x = toX;
    pawn->y = toY;

    return true;
}

void GameBoard::printMap(int *map)
{

    // for (int x = 0; x < boardWidth-1; x++)
    // {
    //     for (int y = 0; y < boardHeigth-1; y++)
    //     {
    //         print(map[x * (boardWidth-1) + y]);
    //         if (map[x * (boardWidth-1) + y] < 10)
    //             print(" ");
    //         print(" ");
    //     }
    //     println();
    // }
}
