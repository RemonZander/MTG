#include <ludoGame.hpp>
#include <Arduino.h>
#include <ludoPlayer.hpp>

void LudoGame::Init()
{
    Serial.println("initializing game");
    this->state.State = LudoGameStates::init;

    this->motion = new MotionController();
    motion->SetPins(MOTOR_A_PINS, MOTOR_B_PINS, limitX, limitY);
    motion->SetPhisicalBoardSize(1000.0, 1000.0, {.x = 10, .y = 10});
    motion->MotorToPos({.x = 5, .y = 5});

    this->Pathfinding = new PathFinding_impoved();

    //actual bord size
    this->bordSize.size = Coordinates_t{.x = 100, .y = 100}; //in mm
    this->bordSize.squareSize = Coordinates_t{.x = 10, .y=10}; //in mm

    //init bord map
    this->map = BoardMap_t{Coordinates_t{.x = BOARD_SIZE_X_LUDO, .y = BOARD_SIZE_Y_LUDO}, .map = {
        {255, 255, 255, 255, 255, 30, 1, 1, 1, 30, 255, 255, 255, 255, 255},
        {255, 1, 1, 1, 1, 30, 1, 30, 1, 30, 1, 1, 1, 1, 255},
        {255, 1, 1, 1, 1, 30, 1, 30, 1, 30, 1, 1, 1, 1, 255},
        {255, 1, 1, 1, 1, 30, 1, 30, 1, 30, 1, 1, 1, 1, 255},
        {255, 1, 1, 1, 1, 30, 1, 30, 1, 30, 1, 1, 1, 1, 255},
        {30, 30, 30, 30, 30, 30, 1, 30, 1, 30, 30, 30, 30, 30, 30},
        {1, 1, 1, 1, 1, 1, 255, 255, 255, 1, 1, 1, 1, 1, 1},
        {1, 30, 30, 30, 30, 30, 255, 255, 255, 30, 30, 30, 30, 30, 1},
        {255, 255, 255, 255, 255, 30, 1, 1, 1, 30, 255, 255, 255, 255, 255},
        {255, 1, 1, 1, 1, 30, 1, 30, 1, 30, 1, 1, 1, 1, 255},
        {255, 1, 1, 1, 1, 30, 1, 30, 1, 30, 1, 1, 1, 1, 255},
        {255, 1, 1, 1, 1, 30, 1, 30, 1, 30, 1, 1, 1, 1, 255},
        {255, 1, 1, 1, 1, 30, 1, 30, 1, 30, 1, 1, 1, 1, 255},
        {30, 30, 30, 30, 30, 30, 1, 30, 1, 30, 30, 30, 30, 30, 30},
        {1, 1, 1, 1, 1, 1, 255, 255, 255, 1, 1, 1, 1, 1, 1}
    }};

    //init gamepath
    this->state.GamePath.push_back(Coordinates_t{6,0});
    this->state.GamePath.push_back(Coordinates_t{7,0});
    this->state.GamePath.push_back(Coordinates_t{8,0});
    this->state.GamePath.push_back(Coordinates_t{8,1});
    this->state.GamePath.push_back(Coordinates_t{8,2});
    this->state.GamePath.push_back(Coordinates_t{8,3});
    this->state.GamePath.push_back(Coordinates_t{8,4});
    this->state.GamePath.push_back(Coordinates_t{8,5});
    this->state.GamePath.push_back(Coordinates_t{9,6});
    this->state.GamePath.push_back(Coordinates_t{10,6});
    this->state.GamePath.push_back(Coordinates_t{11,6});
    this->state.GamePath.push_back(Coordinates_t{12,6});
    this->state.GamePath.push_back(Coordinates_t{13,6});
    this->state.GamePath.push_back(Coordinates_t{14,6});
    this->state.GamePath.push_back(Coordinates_t{14,7});
    this->state.GamePath.push_back(Coordinates_t{14,8});
    this->state.GamePath.push_back(Coordinates_t{13,8});
    this->state.GamePath.push_back(Coordinates_t{12,8});
    this->state.GamePath.push_back(Coordinates_t{11,8});
    this->state.GamePath.push_back(Coordinates_t{10,8});
    this->state.GamePath.push_back(Coordinates_t{9,8});
    this->state.GamePath.push_back(Coordinates_t{8,9});
    this->state.GamePath.push_back(Coordinates_t{8,10});
    this->state.GamePath.push_back(Coordinates_t{8,11});
    this->state.GamePath.push_back(Coordinates_t{8,12});
    this->state.GamePath.push_back(Coordinates_t{8,13});
    this->state.GamePath.push_back(Coordinates_t{8,14});
    this->state.GamePath.push_back(Coordinates_t{7,14});
    this->state.GamePath.push_back(Coordinates_t{6,14});
    this->state.GamePath.push_back(Coordinates_t{6,13});
    this->state.GamePath.push_back(Coordinates_t{6,12});
    this->state.GamePath.push_back(Coordinates_t{6,11});
    this->state.GamePath.push_back(Coordinates_t{6,10});
    this->state.GamePath.push_back(Coordinates_t{6,9});
    this->state.GamePath.push_back(Coordinates_t{5,8});
    this->state.GamePath.push_back(Coordinates_t{4,8});
    this->state.GamePath.push_back(Coordinates_t{3,8});
    this->state.GamePath.push_back(Coordinates_t{2,8});
    this->state.GamePath.push_back(Coordinates_t{1,8});
    this->state.GamePath.push_back(Coordinates_t{0,8});
    this->state.GamePath.push_back(Coordinates_t{0,7});
    this->state.GamePath.push_back(Coordinates_t{0,6});
    this->state.GamePath.push_back(Coordinates_t{1,6});
    this->state.GamePath.push_back(Coordinates_t{2,6});
    this->state.GamePath.push_back(Coordinates_t{3,6});
    this->state.GamePath.push_back(Coordinates_t{4,6});
    this->state.GamePath.push_back(Coordinates_t{5,6});
    this->state.GamePath.push_back(Coordinates_t{6,5});
    this->state.GamePath.push_back(Coordinates_t{6,4});
    this->state.GamePath.push_back(Coordinates_t{6,3});
    this->state.GamePath.push_back(Coordinates_t{6,2});
    this->state.GamePath.push_back(Coordinates_t{6,1});

    //init players with pawns
    this->players = new std::vector<Player<LudoPlayerState_t, LudoPawnState_t>*>{
        new LudoPlayer<LudoPlayerState_t,LudoPawnState_t>(0, new std::vector<Pawn<LudoPawnState_t>*>{ 
        new Pawn<LudoPawnState_t>(0, Coordinates_t{1, 1}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsHome = false, .HasFinished = false}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{3, 1}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsHome = false, .HasFinished = false}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{1, 3}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsHome = false, .HasFinished = false}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{3, 3}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsHome = false, .HasFinished = false}) 
        }, LudoPlayerState_t{.userInputModule = LudoInputModule(0x08), .HasPawnOnboard = false}),
        new LudoPlayer<LudoPlayerState_t, LudoPawnState_t>(1, new std::vector<Pawn<LudoPawnState_t>*>{ 
        new Pawn<LudoPawnState_t>(0, Coordinates_t{11, 1}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsHome = false, .HasFinished = false}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{13, 1}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsHome = false, .HasFinished = false}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{11, 3}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsHome = false, .HasFinished = false}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{13, 3}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsHome = false, .HasFinished = false}) 
        }, LudoPlayerState_t{.userInputModule = LudoInputModule(0x0B), .HasPawnOnboard = false}),
        new LudoPlayer<LudoPlayerState_t, LudoPawnState_t>(2, new std::vector<Pawn<LudoPawnState_t>*>{ 
        new Pawn<LudoPawnState_t>(0, Coordinates_t{1, 11}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsHome = false, .HasFinished = false}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{3, 11}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsHome = false, .HasFinished = false}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{1, 13}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsHome = false, .HasFinished = false}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{3, 13}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsHome = false, .HasFinished = false})  
        }, LudoPlayerState_t{.userInputModule = LudoInputModule(0x0A), .HasPawnOnboard = false}),
        new LudoPlayer<LudoPlayerState_t, LudoPawnState_t>(3, new std::vector<Pawn<LudoPawnState_t>*>{ 
        new Pawn<LudoPawnState_t>(0, Coordinates_t{11, 11}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsHome = false, .HasFinished = false}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{13, 11}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsHome = false, .HasFinished = false}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{11, 13}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsHome = false, .HasFinished = false}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{13, 13}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsHome = false, .HasFinished = false}) 
        }, LudoPlayerState_t{.userInputModule = LudoInputModule(0x09), .HasPawnOnboard = false})};

    Serial.println("initialization done, starting game...");
    this->state.State = LudoGameStates::Player1;
    currentPlayer = 0;
};

void LudoGame::GameLoop()
{
    while (this->state.State != LudoGameStates::stopped || this->state.State != LudoGameStates::error)
    {       
        (*this->players)[currentPlayer]->DoTurn();
        

        int selectedPawn = -1;
        for (size_t i = 0; i < (*this->players)[currentPlayer]->Pawns->size(); i++)
        {
            if ((*(*this->players)[currentPlayer]->Pawns)[i]->State.IsSelected) 
            {
                selectedPawn = i;
                break;
            }
        }
        if (selectedPawn != -1)
        {
            std::vector<Pawn<LudoPawnState_t>*> allPawns;
            for (size_t i = 0; i < this->players->size(); i++) for (size_t j = 0; j < (*this->players)[i]->Pawns->size(); j++) allPawns.push_back((*(*this->players)[i]->Pawns)[j]);

            Coordinates_t nextPos;
            if ((*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.IsHome)
            {
                nextPos = currentPlayer == 0 ? this->state.GamePath[40] : 
                currentPlayer == 1 ? this->state.GamePath[1] : 
                currentPlayer == 2 ? this->state.GamePath[14] : this->state.GamePath[27];

                this->motion->ExecutePath(this->Pathfinding->findPath((*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->squareCords, nextPos, this->map, &allPawns, 5));
            }
            
            if (!(*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.HasFinished)
            {
                nextPos = this->state.GamePath[(*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath + (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.Steps];
                this->motion->ExecutePath(this->Pathfinding->findPath((*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->squareCords, nextPos, this->map, &allPawns, 5));
            }


            (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State = LudoPawnState_t{.IsSelected = false, .Steps = 0};
        }

        if (this->state.State == LudoGameStates::Player4)
        {
            this->state.State = LudoGameStates::Player1;
            currentPlayer++;
        }
        else 
        {
            this->state.State = (LudoGameStates)((uint8_t)LudoGameStates::Player1 + 1);
            currentPlayer = 0;
        }
    }
};