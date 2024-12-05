#include <ludoGame.hpp>
#include <Arduino.h>
#include <ludoPlayer.hpp>

void LudoGame::Init()
{
    Serial.println("initializing game");
    this->state = LudoGameStates::init;

    this->motion = new MotionController();
    motion->SetPins(MOTOR_A_PINS, MOTOR_B_PINS, limitX, limitY);
    motion->SetPhisicalBoardSize(1000.0, 1000.0, {.x = 10, .y = 10});
    motion->MotorToPos({.x = 5, .y = 5});

    this->Pathfinding = new PathFinding_impoved();

    //actual bord size
    this->bordSize.size = Coordinates_t{.x = 100, .y = 100}; //in mm
    this->bordSize.squareSize = Coordinates_t{.x = 10, .y=10}; //in mm

    //init bord map
    this->map = BoardMap_t{Coordinates_t{.x = BOARD_SIZE_X_LUDO, .y = BOARD_SIZE_Y_LUDO}, BOARD_SIZE_X_LUDO, BOARD_SIZE_Y_LUDO};

    //init players with pawns
    this->players = new std::vector<Player<LudoPlayerState_t, LudoPawnState_t>*>{
        new LudoPlayer<LudoPlayerState_t,LudoPawnState_t>(0, new std::vector<Pawn<LudoPawnState_t>*>{ 
        new Pawn<LudoPawnState_t>(0, Coordinates_t{10, 10}, LudoPawnState_t{.IsSelected = false, .Steps = 0}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{10, 10}, LudoPawnState_t{.IsSelected = false, .Steps = 0}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{10, 10}, LudoPawnState_t{.IsSelected = false, .Steps = 0}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{10, 10}, LudoPawnState_t{.IsSelected = false, .Steps = 0}) 
        }, LudoPlayerState_t{.userInputModule = LudoInputModule(0x08), .HasPawnOnboard = false}),
        new LudoPlayer<LudoPlayerState_t, LudoPawnState_t>(1, new std::vector<Pawn<LudoPawnState_t>*>{ 
        new Pawn<LudoPawnState_t>(0, Coordinates_t{20, 20}, LudoPawnState_t{.IsSelected = false, .Steps = 0}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{20, 20}, LudoPawnState_t{.IsSelected = false, .Steps = 0}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{20, 20}, LudoPawnState_t{.IsSelected = false, .Steps = 0}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{20, 20}, LudoPawnState_t{.IsSelected = false, .Steps = 0}) 
        }, LudoPlayerState_t{.userInputModule = LudoInputModule(0x0B), .HasPawnOnboard = false}),
        new LudoPlayer<LudoPlayerState_t, LudoPawnState_t>(2, new std::vector<Pawn<LudoPawnState_t>*>{ 
        new Pawn<LudoPawnState_t>(0, Coordinates_t{30, 30}, LudoPawnState_t{.IsSelected = false, .Steps = 0}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{30, 30}, LudoPawnState_t{.IsSelected = false, .Steps = 0}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{30, 30}, LudoPawnState_t{.IsSelected = false, .Steps = 0}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{30, 30}, LudoPawnState_t{.IsSelected = false, .Steps = 0})  
        }, LudoPlayerState_t{.userInputModule = LudoInputModule(0x0A), .HasPawnOnboard = false}),
        new LudoPlayer<LudoPlayerState_t, LudoPawnState_t>(3, new std::vector<Pawn<LudoPawnState_t>*>{ 
        new Pawn<LudoPawnState_t>(0, Coordinates_t{40, 40}, LudoPawnState_t{.IsSelected = false, .Steps = 0}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{40, 40}, LudoPawnState_t{.IsSelected = false, .Steps = 0}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{40, 40}, LudoPawnState_t{.IsSelected = false, .Steps = 0}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{40, 40}, LudoPawnState_t{.IsSelected = false, .Steps = 0}) 
        }, LudoPlayerState_t{.userInputModule = LudoInputModule(0x09), .HasPawnOnboard = false})};

    Serial.println("initialization done, starting game...");
    this->state = LudoGameStates::Player1;
    currentPlayer = 0;
};

void LudoGame::GameLoop()
{
    while (this->state != LudoGameStates::stopped || this->state != LudoGameStates::error)
    {       
        (*this->players)[currentPlayer]->DoTurn();
        

        int selectedPawn = 0;
        for (size_t i = 0; i < (*this->players)[currentPlayer]->Pawns->size(); i++)
        {
            if ((*(*this->players)[currentPlayer]->Pawns)[i]->State.IsSelected) 
            {
                selectedPawn = i;
                break;
            }
        }

        this->motion->ExecutePath(this->Pathfinding->findPath((*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->squareCords, Coordinates_t{40, 40}, this->map, (*this->players)[currentPlayer]->Pawns, 5));

        (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State = LudoPawnState_t{.IsSelected = false, .Steps = 0};

        if (this->state == LudoGameStates::Player4) 
        {
            this->state = LudoGameStates::Player1;
            currentPlayer++;
        }
        else 
        {
            this->state = (LudoGameStates)((uint8_t)LudoGameStates::Player1 + 1);
            currentPlayer = 0;
        }
    }
};