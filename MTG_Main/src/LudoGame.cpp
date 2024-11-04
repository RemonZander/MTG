#include <ludoGame.hpp>
#include <typedefs.h>
#include <LudoUserInput.hpp>
#include <Arduino.h>

void LudoGame::Init()
{
    //init motioncontroller
    //todo, waiting for implementation

    Serial.println("initializing game");
    return;
    GameBase::state = LudoGameStates::init;

    //actual bord size
    GameBase::bordSize.size = Coordinates_t{.x = 100, .y = 100}; //in mm
    GameBase::bordSize.squareSize = Coordinates_t{.x = 10, .y=10}; //in mm

    //init bord map
    GameBase::map = BoardMap_t{Coordinates_t{.x = BOARD_SIZE_X_LUDO, .y = BOARD_SIZE_Y_LUDO}, BOARD_SIZE_X_LUDO, BOARD_SIZE_Y_LUDO};

    //init players with pawns
    GameBase::players = new std::vector<Player<LudoInputModule, int>*>{
        new Player<LudoInputModule, int>(0, new std::vector<Pawn<int>*>{ 
        new Pawn<int>(0, Coordinates_t{10, 10}, 0),
        new Pawn<int>(0, Coordinates_t{10, 10}, 0),
        new Pawn<int>(0, Coordinates_t{10, 10}, 0),
        new Pawn<int>(0, Coordinates_t{10, 10}, 0) 
        }, LudoInputModule(0x08)),
        new Player<LudoInputModule, int>(1, new std::vector<Pawn<int>*>{ 
        new Pawn<int>(0, Coordinates_t{20, 20}, 0),
        new Pawn<int>(0, Coordinates_t{20, 20}, 0),
        new Pawn<int>(0, Coordinates_t{20, 20}, 0),
        new Pawn<int>(0, Coordinates_t{20, 20}, 0) 
        }, LudoInputModule(0x0B)),
        new Player<LudoInputModule, int>(2, new std::vector<Pawn<int>*>{ 
        new Pawn<int>(0, Coordinates_t{30, 30}, 0),
        new Pawn<int>(0, Coordinates_t{30, 30}, 0),
        new Pawn<int>(0, Coordinates_t{30, 30}, 0),
        new Pawn<int>(0, Coordinates_t{30, 30}, 0)  
        }, LudoInputModule(0x0A)),
        new Player<LudoInputModule, int>(3, new std::vector<Pawn<int>*>{ 
        new Pawn<int>(0, Coordinates_t{40, 40}, 0),
        new Pawn<int>(0, Coordinates_t{40, 40}, 0),
        new Pawn<int>(0, Coordinates_t{40, 40}, 0),
        new Pawn<int>(0, Coordinates_t{40, 40}, 0) 
        }, LudoInputModule(0x09))};

    Serial.println("initialization done, starting game...");
    GameBase::state = LudoGameStates::Player1;
};

void LudoGame::GameLoop()
{
    while (GameBase::state != LudoGameStates::stopped || GameBase::state != LudoGameStates::error)
    {
        GameBase::state = (*GameBase::players)[0]->DoTurn();
        delay(1000);
    }
};