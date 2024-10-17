#include <ludoGame.hpp>
#include <typedefs.h>

void LudoGame::Init()
{
    //init motioncontroller

    //actual bord size
    GameBase::bordSize.size = Coordinates_t{.x = 100, .y = 100}; //in mm
    GameBase::bordSize.squareSize = Coordinates_t{.x = 10, .y=10}; //in mm

    GameBase::players = {
        new Player<int, int>(0, {new Pawn<int>(0, Coordinates_t{.x = 10, .y = 10}, 0)}, 1),
        new Player<int ,int>(0, {new Pawn<int>(0, Coordinates_t{.x = 10, .y = 10}, 0)}, 1),
        new Player<int, int>(0, {new Pawn<int>(0, Coordinates_t{.x = 10, .y = 10}, 0)}, 1),
    };
};