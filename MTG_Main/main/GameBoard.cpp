#include "./GameBoard.hpp"
#include <math.h>

GameBoard::GameBoard(int width, int height, int originOffsetX = 0, int originOffsetY = 0) : boardHeigth(height), boardWidth(width), originOffsetX(originOffsetX), originOffsetY(originOffsetY)
{
    Serial.println("GameBoard init");
    this->controller = MoterController();
    this->controller.HomeMotors();

    ratioX = this->controller.getMaxBoardX() / (width-1);
    ratioY = this->controller.getMaxBoardY() / (height-1);

    //cursed line. Circulair references. Lot of shared states
    path = new PathFinding(this);
}

GameBoard::~GameBoard()
{
        Serial.println("GameBoard, I say good day");

    if(path != nullptr)
        delete(path);
}


bool GameBoard::movePawn(int fromX, int fromY, int toX, int toY)
{
    /*
   Serial.println("Moving to");
   Serial.print(toX);
   Serial.print("|");
   Serial.print(toY);
    */


    fromX += originOffsetX;
    fromY += originOffsetY;
    toX += originOffsetX;
    toY += originOffsetY;

    if ((fromX > boardWidth-1 || fromY > boardHeigth-1) || (toX > boardWidth-1 || toY > boardHeigth-1))
    {
        Serial.println("Oopsie woospie ik mag daar niet naartoe");
        Serial.print(fromX);
        Serial.print(" | ");
        Serial.print(fromY);
        Serial.print(" | ");
        Serial.print(toX);
        Serial.print(" | ");
        Serial.println(fromY);
        return false;
    }
    if ((fromX < 0 || fromY < 0) || (toX < 0 || toY < 0))
    {
        Serial.println("Oopsie woospie dat is te negatief ");
        Serial.print(fromX);
        Serial.print(" | ");
        Serial.print(fromY);
        Serial.print(" | ");
        Serial.print(toX);
        Serial.print(" | ");
        Serial.println(fromY);
        return false;
    }

   //Serial.println("test");

    int sourceX = (fromX * ratioX);
    int sourceY = (fromY * ratioY);

    int destX = (toX * ratioX);
    int destY = (toY * ratioY);


    this->controller.magnetOff();
    this->controller.MotorToPos(sourceX, sourceY);
    this->controller.magnetOn();
    this->controller.MotorToPos(destX, destY);
    
    //delay(500);
    this->controller.magnetOff();
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

    for (int x = 0; x < boardWidth-1; x++)
    {
        for (int y = 0; y < boardHeigth-1; y++)
        {
            Serial.print(map[x * (boardWidth-1) + y]);
            if (map[x * (boardWidth-1) + y] < 10)
                Serial.print(" ");
            Serial.print(" ");
        }
        Serial.println();
    }
}
