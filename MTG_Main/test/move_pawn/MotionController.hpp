#ifndef MOTIONCONTROLLER_H
#define MOTIONCONTROLLER_H

#include "../../main/typedefs.h"
#include <stdint.h>

class MotionController
{
public:
    MotionController();
    ~MotionController();

    /** SetPhisicalBoardSize
     * define the size of the game board.
     * 
     * args:
     * - x: x dimantion in mm of the playing field
     * - y: y dimantion in mm of the playing field
     * - offsetX: x offset in mm of the playing field from center
     * - offsetY: y offset in mm of the playing field from center
     * - boardSize: x and y size in squaers of the playing field
     */
    void SetPhisicalBoardSize(float x, float y, float offsetX, float offsetY, Coordinates_t boardSize);

    /** ExecutePath
     * execute a path as genereted by the pathfinding
     * 
     * args:
     * - path: the path
     */
    bool ExecutePath(pathfinding_path_t path);

    /** MotorToPos
     * move magnet to a square in the playing field
     * 
     * args:
     * - pos: the square to move to
     */
    bool MotorToPos(Coordinates_t pos);

    /** SetMagnet
     * enable or disable electromagnet
     * 
     * args:
     * - state: true: magnet on, false: magnet off
     */
    void SetMagnet(bool state);

private:
    Coordinates_t _boardSize = {0, 0};
    float _phisicalBoardSizeX = 0.0;
    float _phisicalBoardSizeY = 0.0;
    Coordinates_t _currPos = {0, 0};
    float _squareSize_x = 0.0;
    float _squareSize_y = 0.0;
    // differance from endstops and (0, 0)
    float _boardOffset_x = 0.0;
    float _boardOffset_y = 0.0;
};

#endif
