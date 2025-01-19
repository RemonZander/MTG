#include "./ludoGame.hpp"

#include "../../main/logger.h"

void LudoGame::Init()
{
    LOG_I("initializing game");
    this->state.State = LudoGameStates::init;

    // nvs_open("storage", NVS_READWRITE, &this->nvsFlashHandle);

    //if GPOI pin high then read from storage

    //this->motion = new MotionController();
    //motion->SetPhisicalBoardSize(240.0, 240.0, 0.0, -5.0, {.x = 15, .y = 15});

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

};

void LudoGame::GameLoop()
{
    int selectedPawn = -1;
    do
    {
        (*this->players)[currentPlayer]->DoTurn();

        for (size_t i = 0; i < (*this->players)[currentPlayer]->Pawns->size(); i++)
        {
            if ((*(*this->players)[currentPlayer]->Pawns)[i]->State.IsSelected) 
            {
                selectedPawn = i;
                break;
            }
        }
    } while (selectedPawn == -1);

    LOG_I("Selected pawn: %i", selectedPawn);

    //if the user has a selected pawn, move it
    if (selectedPawn != -1)
    {
        auto selectedPawnObj = (*(*this->players)[currentPlayer]->Pawns)[selectedPawn];
        //if pawn is at start move it
        Coordinates_t nextPos = {.x = 0, .y = 0};
        if (selectedPawnObj->State.IsAtStart)
        {
            switch (currentPlayer)
            {
                case 0:
                    nextPos = this->state.GamePath[1];
                    selectedPawnObj->State.CurrentGamePath = 1;
                    break;
                case 1:
                    nextPos = this->state.GamePath[15];
                    selectedPawnObj->State.CurrentGamePath = 15;
                    break;
                case 2:
                    nextPos = this->state.GamePath[29];
                    selectedPawnObj->State.CurrentGamePath = 29;
                    break;
                case 3:
                    nextPos = this->state.GamePath[43];
                    selectedPawnObj->State.CurrentGamePath = 43;
                    break;
            }

            selectedPawnObj->State.IsAtStart = false;
            (*this->players)[currentPlayer]->State.HasPawnOnboard = true;

            this->Pathfinding->findPath(selectedPawnObj->squareCords, nextPos, this->map, &this->state.allPawns, 255);
            LOG_I("Moving pawn %u from x: %u y: %u to x: %u y: %u", selectedPawn, selectedPawnObj->squareCords.x, selectedPawnObj->squareCords.y, nextPos.x, nextPos.y);

            selectedPawnObj->squareCords = nextPos;


        }

        selectedPawnObj->State.IsSelected = false;
        selectedPawnObj->State.Steps = 0;
        //this->SaveGame();
    }
};

// void LudoGame::SaveGame()
// {
//     LOG_I("Saving game...");
//     nvs_handle_t nvsFlashHandle;
//     size_t size = 1;
//     int err = nvs_open("storage", NVS_READWRITE, &nvsFlashHandle);
//     if (err != 0){
//         printf("faild to open vns (%i)\n", err);
//     }
//     err = nvs_set_blob(nvsFlashHandle, "LudoGameState", (void*)&this->state.State, size);
//     if (err != 0){
//         printf("LudoGameState = error %i\n", err);
//     }
//     nvs_commit(nvsFlashHandle);
//     err = nvs_set_blob(nvsFlashHandle, "currentPlayer", (void*)&this->currentPlayer, size);
//     if (err != 0){
//         printf("currentPlayer = error %i\n", err);
//     }
//     nvs_commit(nvsFlashHandle);

//     char str[128];

//     for (size_t i = 0; i < this->players->size(); i++)
//     {
//         snprintf(&str[0], 128, "player%u", i);
//         err = nvs_set_blob(nvsFlashHandle, &str[0], (void*)&(*this->players)[i]->ID, size);
//         if (err != 0){
//             printf("%s = error %i\n", &str[0], err);
//         }
//         nvs_commit(nvsFlashHandle);
//         snprintf(&str[0], 128, "player%uHasP", i);
//         err = nvs_set_blob(nvsFlashHandle, &str[0], (void*)&(*this->players)[i]->State.HasPawnOnboard, size);
//         if (err != 0){
//             printf("%s = error %i\n", &str[0], err);
//         }
//         nvs_commit(nvsFlashHandle);

//         for (size_t j = 0; j < (*this->players)[i]->Pawns->size(); j++)
//         {
//             // snprintf(&str[0], 128, "player%upawn%u", i, j);
//             // nvs_set_i32(nvsFlashHandle, &str[0], (int32_t)(*(*this->players)[i]->Pawns)[j]->State.ID);
//             // nvs_commit(nvsFlashHandle);
//             snprintf(&str[0], 128, "p%upn%usCords.x", i, j);
//             err = nvs_set_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->squareCords.x, size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             }
//             snprintf(&str[0], 128, "p%upn%usCords.y", i, j);
//             err = nvs_set_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->squareCords.y, size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             }
//             snprintf(&str[0], 128, "p%upn%uIsSelected", i, j);
//             err = nvs_set_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.IsSelected, size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             }
//             nvs_commit(nvsFlashHandle);
//             snprintf(&str[0], 128, "p%upn%uSteps", i, j);
//             err = nvs_set_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.Steps, size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             }
//             nvs_commit(nvsFlashHandle);
//             snprintf(&str[0], 128, "p%upn%uIsAtStart", i, j);
//             err = nvs_set_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.IsAtStart, size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             }
//             nvs_commit(nvsFlashHandle);
//             snprintf(&str[0], 128, "p%upn%uIsInHome", i, j);
//             err = nvs_set_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.IsInHome, size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             }
//             nvs_commit(nvsFlashHandle);
//             snprintf(&str[0], 128, "p%upn%uFinished", i, j);
//             err = nvs_set_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.HasFinished, size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             }
//             nvs_commit(nvsFlashHandle);
//             snprintf(&str[0], 128, "p%upn%ustartPosX", i, j);
//             err = nvs_set_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.startPos.x, size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             }
//             nvs_commit(nvsFlashHandle);
//             snprintf(&str[0], 128, "p%upn%ustartPosY", i, j);
//             err = nvs_set_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.startPos.y, size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             }
//             nvs_commit(nvsFlashHandle);
//             snprintf(&str[0], 128, "p%upn%uCGP", i, j);
//             err = nvs_set_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.CurrentGamePath, size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             }
//             nvs_commit(nvsFlashHandle);
//         }
//     }

//     nvs_commit(nvsFlashHandle);
//     nvs_close(nvsFlashHandle);
//     LOG_I("Game saved!");
// }

// void LudoGame::RestoreGame()
// {
//     LOG_I("Restoring game...");
//     nvs_handle_t nvsFlashHandle;
//     size_t size = 1;
//     int err = nvs_open("storage", NVS_READWRITE, &nvsFlashHandle);
//     if (err != 0){
//         printf("faild to open vns (%i)\n", err);
//     }
//     err = nvs_get_blob(nvsFlashHandle, "LudoGameState", (void*)&this->state.State, &size);
//     if (err != 0){
//         printf("LudoGameState = error %i\n", err);
//     } else {
//         printf("LudoGameState = %i\n", (int8_t)this->state.State);
//     }
//     err = nvs_get_blob(nvsFlashHandle, "currentPlayer", (void*)&this->currentPlayer, &size);
//     if (err != 0){
//         printf("currentPlayer = error %i\n", err);
//     } else {
//         printf("currentPlayer = %u\n", (uint8_t)this->currentPlayer);
//     }

//     char str[128];

//     for (size_t i = 0; i < this->players->size(); i++)
//     {
//         snprintf(&str[0], 128, "player%u", i);
//         err = nvs_get_blob(nvsFlashHandle, &str[0], (void*)&(*this->players)[i]->ID, &size);
//         if (err != 0){
//             printf("%s = error %i\n", &str[0], err);
//         } else {
//             printf("%s = %u\n", &str[0], (uint8_t)(*this->players)[i]->ID);
//         }
//         snprintf(&str[0], 128, "player%uHasP", i);
//         err = nvs_get_blob(nvsFlashHandle, &str[0], (void*)&(*this->players)[i]->State.HasPawnOnboard, &size);
//         if (err != 0){
//             printf("%s = error %i\n", &str[0], err);
//         }
//         nvs_commit(nvsFlashHandle);

//         for (size_t j = 0; j < (*this->players)[i]->Pawns->size(); j++)
//         {
//             // snprintf(&str[0], 128, "player%upawn%u", i, j);
//             // err = nvs_get_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.ID, &size);
//             snprintf(&str[0], 128, "p%upn%usCords.x", i, j);
//             err = nvs_get_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->squareCords.x, &size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             } else {
//                 printf("%s = %u\n", &str[0], (uint8_t)(*(*this->players)[i]->Pawns)[j]->squareCords.x);
//             }
//             snprintf(&str[0], 128, "p%upn%usCords.y", i, j);
//             err = nvs_get_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->squareCords.y, &size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             } else {
//                 printf("%s = %u\n", &str[0], (uint8_t)(*(*this->players)[i]->Pawns)[j]->squareCords.y);
//             }
//             snprintf(&str[0], 128, "p%upn%uIsSelected", i, j);
//             err = nvs_get_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.IsSelected, &size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             } else {
//                 printf("%s = %u\n", &str[0], (uint8_t)(*(*this->players)[i]->Pawns)[j]->State.IsSelected);
//             }
//             snprintf(&str[0], 128, "p%upn%uSteps", i, j);
//             err = nvs_get_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.Steps, &size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             } else {
//                 printf("%s = %u\n", &str[0], (uint8_t)(*(*this->players)[i]->Pawns)[j]->State.Steps);
//             }
//             snprintf(&str[0], 128, "p%upn%uIsAtStart", i, j);
//             err = nvs_get_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.IsAtStart, &size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             } else {
//                 printf("%s = %u\n", &str[0], (uint8_t)(*(*this->players)[i]->Pawns)[j]->State.IsAtStart);
//             }
//             snprintf(&str[0], 128, "p%upn%uIsInHome", i, j);
//             err = nvs_get_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.IsInHome, &size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             } else {
//                 printf("%s = %u\n", &str[0], (uint8_t)(*(*this->players)[i]->Pawns)[j]->State.IsInHome);
//             }
//             snprintf(&str[0], 128, "p%upn%uFinished", i, j);
//             err = nvs_get_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.HasFinished, &size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             } else {
//                 printf("%s = %u\n", &str[0], (uint8_t)(*(*this->players)[i]->Pawns)[j]->State.HasFinished);
//             }
//             snprintf(&str[0], 128, "p%upn%ustartPosX", i, j);
//             err = nvs_get_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.startPos.x, &size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             } else {
//                 printf("%s = %u\n", &str[0], (uint8_t)(*(*this->players)[i]->Pawns)[j]->State.startPos.x);
//             }
//             snprintf(&str[0], 128, "p%upn%ustartPosY", i, j);
//             err = nvs_get_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.startPos.y, &size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             } else {
//                 printf("%s = %u\n", &str[0], (uint8_t)(*(*this->players)[i]->Pawns)[j]->State.startPos.y);
//             }
//             snprintf(&str[0], 128, "p%upn%uCGP", i, j);
//             err = nvs_get_blob(nvsFlashHandle, &str[0], (void*)&(*(*this->players)[i]->Pawns)[j]->State.CurrentGamePath, &size);
//             if (err != 0){
//                 printf("%s = error %i\n", &str[0], err);
//             } else {
//                 printf("%s = %u\n", &str[0], (uint8_t)(*(*this->players)[i]->Pawns)[j]->State.CurrentGamePath);
//             }
//         }
//     }

//     for (int i = 0; i <  this->state.allPawns.size(); i++)
//     {
//         if (this->state.allPawns[i]->squareCords.x != this->state.allPawns[i]->State.startPos.x
//             || this->state.allPawns[i]->squareCords.y != this->state.allPawns[i]->State.startPos.y)
//         {
//             LOG_I("Moving pawn %u from x: %u y: %u to x: %u y: %u", i, 
//                 this->state.allPawns[i]->State.startPos.x, this->state.allPawns[i]->State.startPos.y, 
//                 this->state.allPawns[i]->squareCords.x, this->state.allPawns[i]->squareCords.y);
//             //this->motion->ExecutePath(this->Pathfinding->findPath(this->state.allPawns[i]->State.startPos, this->state.allPawns[i]->squareCords, this->map, &this->state.allPawns, 255));
//         }
//     }

//     nvs_close(nvsFlashHandle);
//     LOG_I("Game restored!");
// }
