#include "./ludoGame.hpp"
#include "../../main/MotionController.hpp"
#include <vector>

#ifdef ARDUONO
#include <Arduino.h>
#endif

#include "../../main/logger.h"

void LudoGame::Init()
{
    LOG_I("initializing game");
    this->state.State = LudoGameStates::init;

    this->motion = new MotionController();
    this->motion->SetPhisicalBoardSize(240.0, 240.0, {.x = 15, .y = 15});
    this->motion->SetStepsPerMM(STEPS_PER_MM, STEPS_PER_MM);
    this->motion->SetPins(MOTOR_A_PINS, MOTOR_B_PINS, limitX, limitY);

    this->Pathfinding = new PathFinding_impoved();

    LOG_I("Created motioncontroller and pathfinding objects");

    //init bord map
    this->map = BoardMap_t{.size = Coordinates_t{.x = BOARD_SIZE_X_LUDO, .y = BOARD_SIZE_Y_LUDO}, .map = {
        {255, 255, 255, 255, 255,  30,   1,   1,   1,  30, 255, 255, 255, 255, 255},
        {255, 255,   1, 255,   1,  30,   1,  30,   1,  30,   1, 255,   1, 255, 255},
        {255,   1,   1,   1,   1,  30,   1,  30,   1,  30,   1,   1,   1,   1, 255},
        {255, 255,   1, 255,   1,  30,   1,  30,   1,  30,   1, 255,   1, 255, 255},
        {255,   1,   1,   1,   1,  30,   1,  30,   1,  30,   1,   1,   1,   1, 255},
        { 30,  30,  30,  30,  30,  30,   1,  30,   1,  30,  30,  30,  30,  30,  30},
        {  1,   1,   1,   1,   1,   1,   1, 255,   1,   1,   1,   1,   1,   1,   1},
        {  1,  30,  30,  30,  30,  30, 255, 255, 255,  30,  30,  30,  30,  30,   1},
        {  1,   1,   1,   1,   1,   1,   1, 255,   1,   1,   1,   1,   1,   1,   1},
        { 30,  30,  30,  30,  30,  30,   1,  30,   1,  30,  30,  30,  30,  30,  30},
        {255,   1,   1,   1,   1,  30,   1,  30,   1,  30,   1,   1,   1,   1, 255},
        {255, 255,   1, 255,   1,  30,   1,  30,   1,  30,   1, 255,   1, 255, 255},
        {255,   1,   1,   1,   1,  30,   1,  30,   1,  30,   1,   1,   1,   1, 255},
        {255, 255,   1, 255,   1,  30,   1,  30,   1,  30,   1, 255,   1, 255, 255},
        {255, 255, 255, 255, 255,  30,   1,   1,   1,  30, 255, 255, 255, 255, 255}
    }};

    LOG_I("Created map");

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

    LOG_I("Created gamepath");


    //init player home positions
    this->state.homePositions.push_back(new std::vector<Coordinates_t>{
        Coordinates_t{1,7},
        Coordinates_t{2,7},
        Coordinates_t{3,7},
        Coordinates_t{4,7},
        Coordinates_t{5,7},
        Coordinates_t{6,7}
        });
    this->state.homePositions.push_back(new std::vector<Coordinates_t>{
        Coordinates_t{7,1},
        Coordinates_t{7,2},
        Coordinates_t{7,3},
        Coordinates_t{7,4},
        Coordinates_t{7,5},
        Coordinates_t{7,6}
        });
    this->state.homePositions.push_back(new std::vector<Coordinates_t>{
        Coordinates_t{13,7},
        Coordinates_t{12,7},
        Coordinates_t{11,7},
        Coordinates_t{10,7},
        Coordinates_t{9,7},
        Coordinates_t{8,7}
        });
    this->state.homePositions.push_back(new std::vector<Coordinates_t>{
        Coordinates_t{7,13},
        Coordinates_t{7,12},
        Coordinates_t{7,11},
        Coordinates_t{7,10},
        Coordinates_t{7,9},
        Coordinates_t{7,8}
        });

    LOG_I("Created homepos");

    //init players with pawns
    this->players = new std::vector<Player<LudoPlayerState_t, LudoPawnState_t>*>{
        new LudoPlayer<LudoPlayerState_t,LudoPawnState_t>(0, new std::vector<Pawn<LudoPawnState_t>*>{ 
        new Pawn<LudoPawnState_t>(0, Coordinates_t{1, 1}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{1, 1}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{3, 1}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{3, 1}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{1, 3}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{1, 3}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{3, 3}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{3, 3}}) 
        }, LudoPlayerState_t{.userInputModule = LudoInputModule(0x08), .HasPawnOnboard = false}),
        new LudoPlayer<LudoPlayerState_t, LudoPawnState_t>(1, new std::vector<Pawn<LudoPawnState_t>*>{ 
        new Pawn<LudoPawnState_t>(0, Coordinates_t{11, 1}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{11, 1}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{13, 1}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{12, 1}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{11, 3}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{11, 3}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{13, 3}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{13, 3}}) 
        }, LudoPlayerState_t{.userInputModule = LudoInputModule(0x0B), .HasPawnOnboard = false}),
        new LudoPlayer<LudoPlayerState_t, LudoPawnState_t>(2, new std::vector<Pawn<LudoPawnState_t>*>{ 
        new Pawn<LudoPawnState_t>(0, Coordinates_t{1, 11}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{1, 11}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{3, 11}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{3, 11}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{1, 13}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{1, 13}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{3, 13}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{3, 13}})  
        }, LudoPlayerState_t{.userInputModule = LudoInputModule(0x0A), .HasPawnOnboard = false}),
        new LudoPlayer<LudoPlayerState_t, LudoPawnState_t>(3, new std::vector<Pawn<LudoPawnState_t>*>{ 
        new Pawn<LudoPawnState_t>(0, Coordinates_t{11, 11}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{11, 11}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{13, 11}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{13, 11}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{11, 13}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{11, 13}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{13, 13}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{13, 13}}) 
        }, LudoPlayerState_t{.userInputModule = LudoInputModule(0x09), .HasPawnOnboard = false})};

        //make a list of all pawns in the game. This list is used for the pathfinding
        for (size_t i = 0; i < this->players->size(); i++) for (size_t j = 0; j < (*this->players)[i]->Pawns->size(); j++) this->state.allPawns.push_back((*(*this->players)[i]->Pawns)[j]);

    LOG_I("initialization done, starting game...");
    this->state.State = LudoGameStates::Player1;
    currentPlayer = 0;
};

void LudoGame::GameLoop()
{
    while (this->state.State != LudoGameStates::stopped || this->state.State != LudoGameStates::error)
    {
        (*this->players)[currentPlayer]->DoTurn();
        

        //get the selected pawn from the user
        uint8_t selectedPawn = -1;
        for (size_t i = 0; i < (*this->players)[currentPlayer]->Pawns->size(); i++)
        {
            if ((*(*this->players)[currentPlayer]->Pawns)[i]->State.IsSelected) 
            {
                selectedPawn = i;
                break;
            }
        }

        //if the user has a selected pawn, move it
        if (selectedPawn != -1)
        {
            //if pawn is at start move it
            Coordinates_t nextPos;
            if ((*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.IsAtStart)
            {
                nextPos = currentPlayer == 0 ? this->state.GamePath[40] : 
                currentPlayer == 1 ? this->state.GamePath[1] : 
                currentPlayer == 2 ? this->state.GamePath[14] : this->state.GamePath[27];

                //this->motion->ExecutePath(this->Pathfinding->findPath((*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->squareCords, nextPos, this->map, &this->state.allPawns, 50));
                (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.IsAtStart = false;
                LOG_I("Moving pawn from start to x: %u y: %u", nextPos.x, nextPos.y);
            }
            
            //try to move pawn if the pawn has not reached the finish
            if (!(*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.HasFinished)
            {
                //check if pawn will be in home lane
                uint8_t nextPathPosition = (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath + (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.Steps;
                if ((currentPlayer == 0 && nextPathPosition > 40 && (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath < 40) ||
                (currentPlayer == 1 && nextPathPosition > 51 && (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath < 51) ||
                (currentPlayer == 2 && nextPathPosition > 14 && (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath < 14) ||
                (currentPlayer == 3 && nextPathPosition > 27 && (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath < 27))
                {
                    LOG_I("Pawn %u is in the home lane", selectedPawn);
                    (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.IsInHome = true;
                    //(*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath = nextPathPosition - 40; //substract 40 because CurrentGamePath is also used for the home lane
                    
                    (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath = currentPlayer == 0 ? (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.Steps - (40 - (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath) :
                    currentPlayer == 1 ? (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.Steps - (51 - (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath) :
                    currentPlayer == 2 ? (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.Steps - (14 - (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath) :
                    (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.Steps - (27 - (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath);
                    
                    
                    nextPos = this->state.homePositions[currentPlayer]->at((*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath);

                    //if pawn is in pos 6 of home lane. The pawn has reached the finish
                    if (nextPathPosition == 6)
                    {
                        (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.HasFinished = true;
                        LOG_I("Pawn %u has finished", selectedPawn);
                    }

                    //check if player has all the pawns at the finish. If so set the gamestate to stopped. The game will stop at the end of this turn
                    if ((*(*this->players)[currentPlayer]->Pawns)[0]->State.HasFinished && (*(*this->players)[currentPlayer]->Pawns)[1]->State.HasFinished &&
                    (*(*this->players)[currentPlayer]->Pawns)[3]->State.HasFinished && (*(*this->players)[currentPlayer]->Pawns)[3]->State.HasFinished)
                    {
                        this->state.State = LudoGameStates::stopped;
                        LOG_I("Player %u has won. The game will be stopped", currentPlayer);
                    }
                }
                //move pawn normally along the game path
                else 
                {
                    LOG_I("moving pawn along board");
                    if ((*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath + (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.Steps > this->state.GamePath.size())
                    {
                        (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath = (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.Steps - (this->state.GamePath.size() - (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath);
                    }
                    (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath += (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.Steps;
                    nextPos = this->state.GamePath[(*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath];
                }
            }

            //check if the next position already has a pawn on it. If so move that pawn back to it's home pos and then continue to move the other pawn
            for (size_t i = 0; i < this->state.allPawns.size(); i++)
            {
                if (this->state.allPawns[i]->squareCords.x == nextPos.x && this->state.allPawns[i]->squareCords.y == nextPos.y)
                {
                    LOG_I("Two pawns are on the same location. One will be returned to it's start position");
                    LOG_I("Moving pawn %u from x: %u y: %u to x: %u y: %u", this->state.allPawns[i]->ID, this->state.allPawns[i]->squareCords.x,
                     this->state.allPawns[i]->squareCords.y, this->state.allPawns[i]->State.startPos.x, this->state.allPawns[i]->State.startPos.y);
                    //this->motion->ExecutePath(this->Pathfinding->findPath(this->state.allPawns[i]->squareCords, this->state.allPawns[i]->State.startPos, this->map, &this->state.allPawns, 50));
                    break;
                }
            }
            //this->motion->ExecutePath(this->Pathfinding->findPath((*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->squareCords, nextPos, this->map, &this->state.allPawns, 50));
            LOG_I("Moving pawn %u from x: %u y: %u to x: %u y: %u", selectedPawn, (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->squareCords.x
            , (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->squareCords.y, nextPos.x, nextPos.y);

            (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.IsSelected = false;
            (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.Steps = 0;
        }

        //set state to next player
        if (this->state.State == LudoGameStates::Player4)
        {
            this->state.State = LudoGameStates::Player1;
            currentPlayer = 0;
            LOG_I("Switching to first player %u", currentPlayer);
        }
        else if (this->state.State != LudoGameStates::stopped && this->state.State != LudoGameStates::error)
        {
            this->state.State = (LudoGameStates)((uint8_t)this->state.State + 1);
            currentPlayer +=1;
            LOG_I("Switching to next player %u", currentPlayer);
        }
        else
        {
            this->state.State = LudoGameStates::stopped;
        }
    }
};