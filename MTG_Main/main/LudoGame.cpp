#include <ludoGame.hpp>

#ifdef ARDUONO
#include <Arduino.h>
#else
#include "nvs_flash.h"
#endif

#include "logger.h"

void LudoGame::Init()
{
    LOG_I("initializing game");
    this->state.State = LudoGameStates::init;

    int err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    // nvs_open("storage", NVS_READWRITE, &this->nvsFlashHandle);

    //if GPOI pin high then read from storage

    this->motion = new MotionController();
    motion->SetPhisicalBoardSize(240.0, 240.0, 0.0, -5.0, {.x = 15, .y = 15});

    this->Pathfinding = new PathFinding_impoved();

    //init bord map
    this->map = BoardMap_t{.size = Coordinates_t{.x = BOARD_SIZE_X_LUDO, .y = BOARD_SIZE_Y_LUDO}, .map = {
        {255, 255, 255, 255, 255,  10,   2,   2,   2,  10, 255, 255, 255, 255, 255},
        {255, 255,   1, 255,  20,  10,   2,  35,   2,  10,  20, 255,   1, 255, 255},
        {255,   1,   1,   1,  20,  10,   2,  35,   2,  10,  20,   1,   1,   1, 255},
        {255, 255,   1, 255,  20,  10,   2,  35,   2,  10,  20, 255,   1, 255, 255},
        {255,  20,  20,  20,  20,  10,   2,  35,   2,  10,  20,  20,  20,  20, 255},
        { 10,  10,  10,  10,  10,  10,   2,  35,   2,  10,  10,  10,  10,  10,  10},
        {  2,   2,   2,   2,   2,   2,   2, 255,   2,   2,   2,   2,   2,   2,   2},
        {  2,  35,  35,  35,  35,  35, 255, 255, 255,  35,  35,  35,  35,  35,   2},
        {  2,   2,   2,   2,   2,   2,   2, 255,   2,   2,   2,   2,   2,   2,   2},
        { 10,  10,  10,  10,  10,  10,   2,  35,   2,  10,  10,  10,  10,  10,  10},
        {255,  20,  20,  20,  20,  10,   2,  35,   2,  10,  20,  20,  20,  20, 255},
        {255, 255,   1, 255,  20,  10,   2,  35,   2,  10,  20, 255,   1, 255, 255},
        {255,   1,   1,   1,  20,  10,   2,  35,   2,  10,  20,   1,   1,   1, 255},
        {255, 255,   1, 255,  20,  10,   2,  35,   2,  10,  20, 255,   1, 255, 255},
        {255, 255, 255, 255, 255,  10,   2,   2,   2,  10, 255, 255, 255, 255, 255}
    }};

    //init gamepath
    this->state.GamePath.push_back(Coordinates_t{0,6});
    this->state.GamePath.push_back(Coordinates_t{1,6});
    this->state.GamePath.push_back(Coordinates_t{2,6});
    this->state.GamePath.push_back(Coordinates_t{3,6});
    this->state.GamePath.push_back(Coordinates_t{4,6});
    this->state.GamePath.push_back(Coordinates_t{5,6});
    this->state.GamePath.push_back(Coordinates_t{6,6});
    this->state.GamePath.push_back(Coordinates_t{6,5});
    this->state.GamePath.push_back(Coordinates_t{6,4});
    this->state.GamePath.push_back(Coordinates_t{6,3});
    this->state.GamePath.push_back(Coordinates_t{6,2});
    this->state.GamePath.push_back(Coordinates_t{6,1});
    this->state.GamePath.push_back(Coordinates_t{6,0});
    this->state.GamePath.push_back(Coordinates_t{7,0});
    this->state.GamePath.push_back(Coordinates_t{8,0});
    this->state.GamePath.push_back(Coordinates_t{8,1});
    this->state.GamePath.push_back(Coordinates_t{8,2});
    this->state.GamePath.push_back(Coordinates_t{8,3});
    this->state.GamePath.push_back(Coordinates_t{8,4});
    this->state.GamePath.push_back(Coordinates_t{8,5});
    this->state.GamePath.push_back(Coordinates_t{8,6});
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
    this->state.GamePath.push_back(Coordinates_t{8,8});
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
    this->state.GamePath.push_back(Coordinates_t{6,8});
    this->state.GamePath.push_back(Coordinates_t{5,8});
    this->state.GamePath.push_back(Coordinates_t{4,8});
    this->state.GamePath.push_back(Coordinates_t{3,8});
    this->state.GamePath.push_back(Coordinates_t{2,8});
    this->state.GamePath.push_back(Coordinates_t{1,8});
    this->state.GamePath.push_back(Coordinates_t{0,8});
    this->state.GamePath.push_back(Coordinates_t{0,7});


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

    //init players with pawns
    this->players = new std::vector<Player<LudoPlayerState_t, LudoPawnState_t>*>{
        new LudoPlayer<LudoPlayerState_t,LudoPawnState_t>(0,
            new std::vector<Pawn<LudoPawnState_t>*>{ 
                new Pawn<LudoPawnState_t>(0, Coordinates_t{1, 1}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{1, 1}}),
                new Pawn<LudoPawnState_t>(0, Coordinates_t{3, 1}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{3, 1}}),
                new Pawn<LudoPawnState_t>(0, Coordinates_t{1, 3}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{1, 3}}),
                new Pawn<LudoPawnState_t>(0, Coordinates_t{3, 3}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{3, 3}})
            },
            LudoPlayerState_t{.userInputModule = LudoInputModule(0x08), .HasPawnOnboard = false}
        ),
        new LudoPlayer<LudoPlayerState_t, LudoPawnState_t>(1,
            new std::vector<Pawn<LudoPawnState_t>*>{ 
                new Pawn<LudoPawnState_t>(0, Coordinates_t{11, 1}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{11, 1}}),
                new Pawn<LudoPawnState_t>(0, Coordinates_t{13, 1}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{13, 1}}),
                new Pawn<LudoPawnState_t>(0, Coordinates_t{11, 3}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{11, 3}}),
                new Pawn<LudoPawnState_t>(0, Coordinates_t{13, 3}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{13, 3}})
            },
            LudoPlayerState_t{.userInputModule = LudoInputModule(0x0B), .HasPawnOnboard = false}
        ),
        new LudoPlayer<LudoPlayerState_t, LudoPawnState_t>(2,
            new std::vector<Pawn<LudoPawnState_t>*>{ 
                new Pawn<LudoPawnState_t>(0, Coordinates_t{13, 13}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{13, 13}}),
                new Pawn<LudoPawnState_t>(0, Coordinates_t{11, 13}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{11, 13}}),
                new Pawn<LudoPawnState_t>(0, Coordinates_t{13, 11}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{13, 11}}),
                new Pawn<LudoPawnState_t>(0, Coordinates_t{11, 11}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{11, 11}})
            },
            LudoPlayerState_t{.userInputModule = LudoInputModule(0x0A), .HasPawnOnboard = false}
        ),
        new LudoPlayer<LudoPlayerState_t, LudoPawnState_t>(3,
            new std::vector<Pawn<LudoPawnState_t>*>{ 
                new Pawn<LudoPawnState_t>(0, Coordinates_t{3, 13}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{3, 13}}),
                new Pawn<LudoPawnState_t>(0, Coordinates_t{1, 13}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{1, 13}}),
                new Pawn<LudoPawnState_t>(0, Coordinates_t{3, 11}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{3, 11}}),
                new Pawn<LudoPawnState_t>(0, Coordinates_t{1, 11}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = true, .IsInHome = false, .HasFinished = false, .startPos = Coordinates_t{1, 11}})
            },
            LudoPlayerState_t{.userInputModule = LudoInputModule(0x09), .HasPawnOnboard = false}
        )
    };

    //make a list of all pawns in the game. This list is used for the pathfinding
    for (size_t i = 0; i < this->players->size(); i++) 
        for (size_t j = 0; j < (*this->players)[i]->Pawns->size(); j++) 
            this->state.allPawns.push_back((*(*this->players)[i]->Pawns)[j]);

    LOG_I("initialization done, starting game...");
    this->state.State = LudoGameStates::Player1;
    currentPlayer = 0;

    // this->motion->ExecutePath(this->Pathfinding->findPath({.x = 8, .y = 6}, {.x = 12, .y = 6}, this->map, &this->state.allPawns, 255));
};

void LudoGame::GameLoop()
{
    while (this->state.State != LudoGameStates::stopped && this->state.State != LudoGameStates::error)
    {
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
            LOG_I("Stopping game...");
            this->state.State = LudoGameStates::stopped;
        }


        //check if player has all the pawns at the finish. If so set the gamestate to stopped. The game will stop at the end of this turn
        for (int i = 0; i < this->players->size(); i++)
        {
            if (   (*(*this->players)[i]->Pawns)[0]->State.HasFinished 
                && (*(*this->players)[i]->Pawns)[1]->State.HasFinished 
                && (*(*this->players)[i]->Pawns)[3]->State.HasFinished 
                && (*(*this->players)[i]->Pawns)[3]->State.HasFinished)
            {
                this->state.State = LudoGameStates::stopped;
                LOG_I("Player %u has won. The game will be stopped", i);
                continue;
            }
        }

        if (this->state.State == LudoGameStates::stopped) continue;

        (*this->players)[currentPlayer]->DoTurn();
        

        //get the selected pawn from the user
        int selectedPawn = -1;
        for (size_t i = 0; i < (*this->players)[currentPlayer]->Pawns->size(); i++)
        {
            if ((*(*this->players)[currentPlayer]->Pawns)[i]->State.IsSelected) 
            {
                selectedPawn = i;
                break;
            }
        }

        LOG_I("Selected pawn: %i", selectedPawn);
        LudoPawnState_t selectedPawnObj = *(*(*this->players)[currentPlayer]->Pawns)[selectedPawn]

        //if the user has a selected pawn, move it
        if (selectedPawn != -1)
        {
            //if pawn is at start move it
            Coordinates_t nextPos;
            if (selectedPawnObj.State.IsAtStart)
            {
                switch (currentPlayer)
                {
                    case 0:
                        nextPos = this->state.GamePath[1];
                        selectedPawnObj.State.CurrentGamePath = 1;
                        break;
                    case 1:
                        nextPos = this->state.GamePath[15];
                        selectedPawnObj.State.CurrentGamePath = 15;
                        break;
                    case 2:
                        nextPos = this->state.GamePath[29];
                        selectedPawnObj.State.CurrentGamePath = 29;
                        break;
                    case 3:
                        nextPos = this->state.GamePath[43];
                        selectedPawnObj.State.CurrentGamePath = 43;
                        break;
                }

                selectedPawnObj.State.IsAtStart = false;
                (*this->players)[currentPlayer]->State.HasPawnOnboard = true;

                LOG_I("Moving pawn from start to x: %u y: %u", nextPos.x, nextPos.y);
            }
            
            //try to move pawn if the pawn has not reached the finish
            else if (!selectedPawnObj.State.HasFinished)
            {
                //check if pawn will be in home lane
                uint8_t nextPathPosition = selectedPawnObj.State.CurrentGamePath + selectedPawnObj.State.Steps;

                if (   selectedPawnObj.State.IsInHome 
                    || (
                        currentPlayer == 0
                        && nextPathPosition > 55
                        && selectedPawnObj.State.CurrentGamePath < 55
                    )
                    || (
                        currentPlayer == 1
                        && nextPathPosition > 13
                        && selectedPawnObj.State.CurrentGamePath < 13
                    )
                    || (
                        currentPlayer == 2
                        && nextPathPosition > 27
                        && selectedPawnObj.State.CurrentGamePath < 27
                    )
                    || (
                        currentPlayer == 3
                        && nextPathPosition > 41
                        && selectedPawnObj.State.CurrentGamePath < 41
                    )
                ) {
                    LOG_I("Pawn %u is in the home lane", selectedPawn);
                    
                    //move pawn into home lane from board
                    if (!selectedPawnObj.State.IsInHome)
                    {
                        switch (currentPlayer)
                        {
                            case 0:
                                selectedPawnObj.State.CurrentGamePath = selectedPawnObj.State.Steps - (55 - selectedPawnObj.State.CurrentGamePath);
                                break;
                            case 1:
                                selectedPawnObj.State.CurrentGamePath = selectedPawnObj.State.Steps - (13 - selectedPawnObj.State.CurrentGamePath);
                                break;
                            case 2:
                                selectedPawnObj.State.CurrentGamePath = selectedPawnObj.State.Steps - (27 - selectedPawnObj.State.CurrentGamePath);
                                break;
                            case 3:
                                selectedPawnObj.State.CurrentGamePath = selectedPawnObj.State.Steps - (41 - selectedPawnObj.State.CurrentGamePath);
                                break;
                        }

                        if (selectedPawnObj.State.CurrentGamePath == 5)
                        {
                            selectedPawnObj.State.HasFinished = true;
                            LOG_I("Pawn %u has finished", selectedPawn);
                            nextPos = this->state.homePositions[currentPlayer]->at(selectedPawnObj.State.CurrentGamePath);
                            // LOG_I("Moving pawn %u from x: %u y: %u to x: %u y: %u", selectedPawn, selectedPawnObj.squareCords.x, selectedPawnObj.squareCords.y, nextPos.x, nextPos.y);
                            // this->motion->ExecutePath(this->Pathfinding->findPath(selectedPawnObj.squareCords, nextPos, this->map, &this->state.allPawns, 255));
                            continue;
                        }
                        //check if pawn cannot move in home lane
                        else if (selectedPawnObj.State.CurrentGamePath + selectedPawnObj.State.Steps > 5)
                        {
                            LOG_I("Pawn %u cannot move into home lane bacause too many steps: %u", selectedPawn, selectedPawnObj.State.Steps);
                            continue;                 
                        }
                        else
                        {
                            selectedPawnObj.State.IsInHome = true;
                            nextPos = this->state.homePositions[currentPlayer]->at(selectedPawnObj.State.CurrentGamePath);
                        }
                    }
                    else
                    {
                        //if pawn is in pos 6 of home lane. The pawn has reached the finish
                        if (selectedPawnObj.State.CurrentGamePath + selectedPawnObj.State.Steps == 5)
                        {
                            selectedPawnObj.State.HasFinished = true;
                            LOG_I("Pawn %u has finished", selectedPawn);
                            selectedPawnObj.State.CurrentGamePath += selectedPawnObj.State.Steps;
                            nextPos = this->state.homePositions[currentPlayer]->at(selectedPawnObj.State.CurrentGamePath);
                            LOG_I("Moving pawn %u from x: %u y: %u to x: %u y: %u", selectedPawn, selectedPawnObj.squareCords.x, selectedPawnObj.squareCords.y, nextPos.x, nextPos.y);
                            this->motion->ExecutePath(this->Pathfinding->findPath(selectedPawnObj.squareCords, nextPos, this->map, &this->state.allPawns, 255));
                            continue;
                        }
                        //check if pawn cannot move in home lane
                        else if (selectedPawnObj.State.CurrentGamePath + selectedPawnObj.State.Steps > 5)
                        {
                            LOG_I("Pawn %u cannot move further in home lane bacause too many steps: %u", selectedPawn, selectedPawnObj.State.Steps);
                            LOG_I("Pawn %u current position in home lane: %u",selectedPawn, selectedPawnObj.State.CurrentGamePath);
                            continue;                 
                        }
                        //move pawn in homelane
                        else
                        {
                            selectedPawnObj.State.CurrentGamePath += selectedPawnObj.State.Steps;
                            selectedPawnObj.State.IsInHome = true;
                            nextPos = this->state.homePositions[currentPlayer]->at(selectedPawnObj.State.CurrentGamePath);
                        } 
                    }  
                }
                //move pawn normally along the game path
                else 
                {
                    LOG_I("Moving pawn along board normally");
                    LOG_I("CurrentGamePath of pawn: %u", selectedPawnObj.State.CurrentGamePath);
                    if (selectedPawnObj.State.CurrentGamePath + selectedPawnObj.State.Steps >= this->state.GamePath.size())
                    {
                        LOG_I("Player rolles over the end of the gamepath");
                        selectedPawnObj.State.CurrentGamePath = selectedPawnObj.State.Steps - (this->state.GamePath.size() - selectedPawnObj.State.CurrentGamePath - 1);
                    }
                    else 
                    {
                        selectedPawnObj.State.CurrentGamePath += selectedPawnObj.State.Steps;
                    }
                    LOG_I("CurrentGamePath of pawn: %u", selectedPawnObj.State.CurrentGamePath);
                    nextPos = this->state.GamePath[selectedPawnObj.State.CurrentGamePath];
                    LOG_I("nextpos: x: %u, y: %u", nextPos.x, nextPos.y);
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
                    this->motion->ExecutePath(this->Pathfinding->findPath(this->state.allPawns[i]->squareCords, this->state.allPawns[i]->State.startPos, this->map, &this->state.allPawns, 255));
                    this->state.allPawns[i]->squareCords = this->state.allPawns[i]->State.startPos;
                    this->state.allPawns[i]->State.IsAtStart = true;
                    this->state.allPawns[i]->State.IsInHome = false;
                    break;
                }
            }

            LOG_I("Moving pawn %u from x: %u y: %u to x: %u y: %u", selectedPawn, selectedPawnObj.squareCords.x, selectedPawnObj.squareCords.y, nextPos.x, nextPos.y);
            this->motion->ExecutePath(this->Pathfinding->findPath(selectedPawnObj.squareCords, nextPos, this->map, &this->state.allPawns, 255));

            selectedPawnObj.squareCords = nextPos;
            selectedPawnObj.State.IsSelected = false;
            selectedPawnObj.State.Steps = 0;
        }
    }
};

void LudoGame::SaveGame()
{
    LOG_I("Saving game...");
    nvs_handle_t nvsFlashHandle;
    int err = nvs_open("storage", NVS_READWRITE, &nvsFlashHandle);
    nvs_set_i32(nvsFlashHandle, "LudoGameState", (int32_t)this->state.State);
    nvs_commit(nvsFlashHandle);
    nvs_set_i32(nvsFlashHandle, "currentPlayer", (int32_t)this->currentPlayer);
    nvs_commit(nvsFlashHandle);

    char str[128];

    for (size_t i = 0; i < BOARD_SIZE_X_LUDO; i++)
    {
        for (size_t j = 0; j < BOARD_SIZE_Y_LUDO; j++)
        {
            snprintf(&str[0], 128, "map[%u,%u]", i, j);
            nvs_set_i32(nvsFlashHandle, &str[0], (int32_t)this->state.State);
            nvs_commit(nvsFlashHandle);
        }
    }

    for (size_t i = 0; i < this->players->size(); i++)
    {
        snprintf(&str[0], 128, "player%u", i);
        nvs_set_i32(nvsFlashHandle, &str[0], (*this->players)[i]->ID);
        nvs_commit(nvsFlashHandle);

        for (size_t j = 0; j < (*this->players)[i]->Pawns->size(); j++)
        {
            // snprintf(&str[0], 128, "player%upawn%u", i, j);
            // nvs_set_i32(nvsFlashHandle, &str[0], (int32_t)(*(*this->players)[i]->Pawns)[j]->State.ID);
            // nvs_commit(nvsFlashHandle);
            snprintf(&str[0], 128, "player%upawn%uIsSelected", i, j);
            nvs_set_i32(nvsFlashHandle, &str[0], (int32_t)(*(*this->players)[i]->Pawns)[j]->State.IsSelected);
            nvs_commit(nvsFlashHandle);
            snprintf(&str[0], 128, "player%upawn%uSteps", i, j);
            nvs_set_i32(nvsFlashHandle, &str[0], (int32_t)(*(*this->players)[i]->Pawns)[j]->State.Steps);
            nvs_commit(nvsFlashHandle);
            snprintf(&str[0], 128, "player%upawn%uIsAtStart", i, j);
            nvs_set_i32(nvsFlashHandle, &str[0], (int32_t)(*(*this->players)[i]->Pawns)[j]->State.IsAtStart);
            nvs_commit(nvsFlashHandle);
            snprintf(&str[0], 128, "player%upawn%uIsInHome", i, j);
            nvs_set_i32(nvsFlashHandle, &str[0], (int32_t)(*(*this->players)[i]->Pawns)[j]->State.IsInHome);
            nvs_commit(nvsFlashHandle);
            snprintf(&str[0], 128, "player%upawn%uHasFinished", i, j);
            nvs_set_i32(nvsFlashHandle, &str[0], (int32_t)(*(*this->players)[i]->Pawns)[j]->State.HasFinished);
            nvs_commit(nvsFlashHandle);
            snprintf(&str[0], 128, "player%upawn%ustartPosX", i, j);
            nvs_set_i32(nvsFlashHandle, &str[0], (int32_t)(*(*this->players)[i]->Pawns)[j]->State.startPos.x);
            nvs_commit(nvsFlashHandle);
            snprintf(&str[0], 128, "player%upawn%ustartPosY", i, j);
            nvs_set_i32(nvsFlashHandle, &str[0], (int32_t)(*(*this->players)[i]->Pawns)[j]->State.startPos.y);
            nvs_commit(nvsFlashHandle);
            snprintf(&str[0], 128, "player%upawn%uCurrentGamePath", i, j);
            nvs_set_i32(nvsFlashHandle, &str[0], (int32_t)(*(*this->players)[i]->Pawns)[j]->State.CurrentGamePath);
            nvs_commit(nvsFlashHandle);
        }
    }

    LOG_I("Game saved!");
}
