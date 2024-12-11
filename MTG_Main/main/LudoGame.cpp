#include <ludoGame.hpp>
#include <Arduino.h>
#include <ludoPlayer.hpp>

void LudoGame::Init()
{
    Serial.println("initializing game");
    this->state.State = LudoGameStates::init;

    this->motion = new MotionController();
    motion->SetPhisicalBoardSize(240.0, 240.0, {.x = 15, .y = 15});
    motion->SetStepsPerMM(STEPS_PER_MM, STEPS_PER_MM);
    motion->SetPins(MOTOR_A_PINS, MOTOR_B_PINS, limitX, limitY);

    this->Pathfinding = new PathFinding_impoved();

    //init bord map
    this->map = BoardMap_t{Coordinates_t{.x = BOARD_SIZE_X_LUDO, .y = BOARD_SIZE_Y_LUDO}, .map = {
        {255, 255, 255, 255, 255, 30, 1, 1, 1, 30, 255, 255, 255, 255, 255},
        {255, 255, 1, 255, 1, 30, 1, 30, 1, 30, 1, 255, 1, 255, 255},
        {255, 1, 1, 1, 1, 30, 1, 30, 1, 30, 1, 1, 1, 1, 255},
        {255, 255, 1, 255, 1, 30, 1, 30, 1, 30, 1, 255, 1, 255, 255},
        {255, 1, 1, 1, 1, 30, 1, 30, 1, 30, 1, 1, 1, 1, 255},
        {30, 30, 30, 30, 30, 30, 1, 30, 1, 30, 30, 30, 30, 30, 30},
        {1, 1, 1, 1, 1, 1, 255, 255, 255, 1, 1, 1, 1, 1, 1},
        {1, 30, 30, 30, 30, 30, 255, 255, 255, 30, 30, 30, 30, 30, 1},
        {1, 1, 1, 1, 1, 1, 255, 255, 255, 1, 1, 1, 1, 1, 1},
        {30, 30, 30, 30, 30, 30, 1, 30, 1, 30, 30, 30, 30, 30, 30},
        {255, 1, 1, 1, 1, 30, 1, 30, 1, 30, 1, 1, 1, 1, 255},
        {255, 255, 1, 255, 1, 30, 1, 30, 1, 30, 1, 255, 1, 255, 255},
        {255, 1, 1, 1, 1, 30, 1, 30, 1, 30, 1, 1, 1, 1, 255},
        {255, 255, 1, 255, 1, 30, 1, 30, 1, 30, 1, 255, 1, 255, 255},
        {255, 255, 255, 255, 255, 30, 1, 1, 1, 30, 255, 255, 255, 255, 255}
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


    //init player home positions
    this->state.homePositions[0] = new std::vector<Coordinates_t>{
        Coordinates_t{1,7},
        Coordinates_t{2,7},
        Coordinates_t{3,7},
        Coordinates_t{4,7},
        Coordinates_t{5,7},
        Coordinates_t{6,7}
        };
    this->state.homePositions[1] = new std::vector<Coordinates_t>{
        Coordinates_t{7,1},
        Coordinates_t{7,2},
        Coordinates_t{7,3},
        Coordinates_t{7,4},
        Coordinates_t{7,5},
        Coordinates_t{7,6}
        };
    this->state.homePositions[2] = new std::vector<Coordinates_t>{
        Coordinates_t{13,7},
        Coordinates_t{12,7},
        Coordinates_t{11,7},
        Coordinates_t{10,7},
        Coordinates_t{9,7},
        Coordinates_t{8,7}
        };
    this->state.homePositions[3] = new std::vector<Coordinates_t>{
        Coordinates_t{7,13},
        Coordinates_t{7,12},
        Coordinates_t{7,11},
        Coordinates_t{7,10},
        Coordinates_t{7,9},
        Coordinates_t{7,8}
        };

    //init players with pawns
    this->players = new std::vector<Player<LudoPlayerState_t, LudoPawnState_t>*>{
        new LudoPlayer<LudoPlayerState_t,LudoPawnState_t>(0, new std::vector<Pawn<LudoPawnState_t>*>{ 
        new Pawn<LudoPawnState_t>(0, Coordinates_t{1, 1}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .homePos = Coordinates_t{1, 1}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{3, 1}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .homePos = Coordinates_t{3, 1}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{1, 3}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .homePos = Coordinates_t{1, 3}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{3, 3}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .homePos = Coordinates_t{3, 3}}) 
        }, LudoPlayerState_t{.userInputModule = LudoInputModule(0x08), .HasPawnOnboard = false}),
        new LudoPlayer<LudoPlayerState_t, LudoPawnState_t>(1, new std::vector<Pawn<LudoPawnState_t>*>{ 
        new Pawn<LudoPawnState_t>(0, Coordinates_t{11, 1}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .homePos = Coordinates_t{11, 1}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{13, 1}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .homePos = Coordinates_t{12, 1}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{11, 3}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .homePos = Coordinates_t{11, 3}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{13, 3}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .homePos = Coordinates_t{13, 3}}) 
        }, LudoPlayerState_t{.userInputModule = LudoInputModule(0x0B), .HasPawnOnboard = false}),
        new LudoPlayer<LudoPlayerState_t, LudoPawnState_t>(2, new std::vector<Pawn<LudoPawnState_t>*>{ 
        new Pawn<LudoPawnState_t>(0, Coordinates_t{1, 11}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .homePos = Coordinates_t{1, 11}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{3, 11}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .homePos = Coordinates_t{3, 11}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{1, 13}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .homePos = Coordinates_t{1, 13}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{3, 13}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .homePos = Coordinates_t{3, 13}})  
        }, LudoPlayerState_t{.userInputModule = LudoInputModule(0x0A), .HasPawnOnboard = false}),
        new LudoPlayer<LudoPlayerState_t, LudoPawnState_t>(3, new std::vector<Pawn<LudoPawnState_t>*>{ 
        new Pawn<LudoPawnState_t>(0, Coordinates_t{11, 11}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .homePos = Coordinates_t{11, 11}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{13, 11}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .homePos = Coordinates_t{13, 11}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{11, 13}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .homePos = Coordinates_t{11, 13}}),
        new Pawn<LudoPawnState_t>(0, Coordinates_t{13, 13}, LudoPawnState_t{.IsSelected = false, .Steps = 0, .IsAtStart = false, .IsInHome = false, .HasFinished = false, .homePos = Coordinates_t{13, 13}}) 
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

        //if the user has a selected pawn, move it
        if (selectedPawn != -1)
        {
            std::vector<Pawn<LudoPawnState_t>*> allPawns;
            for (size_t i = 0; i < this->players->size(); i++) for (size_t j = 0; j < (*this->players)[i]->Pawns->size(); j++) allPawns.push_back((*(*this->players)[i]->Pawns)[j]);

            Coordinates_t nextPos;
            if ((*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.IsAtStart)
            {
                nextPos = currentPlayer == 0 ? this->state.GamePath[40] : 
                currentPlayer == 1 ? this->state.GamePath[1] : 
                currentPlayer == 2 ? this->state.GamePath[14] : this->state.GamePath[27];

                this->motion->ExecutePath(this->Pathfinding->findPath((*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->squareCords, nextPos, this->map, &allPawns, 255));
                (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.IsAtStart = false;
            }
            
            //try to move pawn if the pawn has not reached the finish
            if (!(*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.HasFinished)
            {
                //check if pawn will be in home lane
                uint8_t nextPathPosition = (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath + (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.Steps;
                if ((currentPlayer = 0 && nextPathPosition > 40 && (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath < 40) ||
                (currentPlayer = 1 && nextPathPosition > 51) ||
                (currentPlayer == 2 && nextPathPosition > 14 && (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath < 14) ||
                (currentPlayer == 3 && nextPathPosition > 27 && (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath < 27))
                {
                    (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.IsInHome = true;
                    (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath = nextPathPosition - 40;
                    nextPos = this->state.homePositions[currentPlayer]->at((*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath);

                    //if pawn is in pos 6 of home lane. The pawn has reached the finish
                    if (nextPathPosition == 6) (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.HasFinished = true;
                }
                //move pawn normally along the game path
                else nextPos = this->state.GamePath[(*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.CurrentGamePath + (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.Steps];
            }

            //check if the next position already has a pawn on it. If so move that pawn back to it's home pos and then continue to move the other pawn
            for (size_t i = 0; i < allPawns.size(); i++)
            {
                if (allPawns[i]->squareCords.x == nextPos.x && allPawns[i]->squareCords.y == nextPos.y)
                {
                    this->motion->ExecutePath(this->Pathfinding->findPath(allPawns[i]->squareCords, allPawns[i]->State.homePos, this->map, &allPawns, 255));
                    break;
                }
            }
            this->motion->ExecutePath(this->Pathfinding->findPath((*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->squareCords, nextPos, this->map, &allPawns, 255));


            (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.IsSelected = false;
            (*(*this->players)[currentPlayer]->Pawns)[selectedPawn]->State.Steps = 0;
        }

        //set state to next player
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