
#include "LudoGame.hpp"

LudoGame::LudoGame(int playerAmount){
    board = new LudoBoard(15, 15, 7, 7);

    for (int i = 0; i < sizeof(UIMs)/sizeof(LudoInputModule); i++) UIMs[i].transmit(UIMCOMMAND_ACKNOWLEDGE);

    this->playerAmount = playerAmount;
    board->pawns = new Pawn*[playerAmount*PAWN_AMOUNT];
    board->pawnAmount = playerAmount*PAWN_AMOUNT;
    for (int i = 0; i < playerAmount; i++) {
    
        players[i] = new Player(i);
        
        for (int pawnIndex = 0; pawnIndex < PAWN_AMOUNT; pawnIndex++) {
            board->pawns[i*PAWN_AMOUNT+pawnIndex] = players[i]->pawns[pawnIndex];
            Serial.println(players[i]->pawns[pawnIndex]->id);
            Serial.println(board->pawns[i*PAWN_AMOUNT+pawnIndex]->id);

        }
    }
    //board->path->updateMap();
    //board->path->printMap();
}


bool LudoGame::nextTurn(){
    int diceroll = UIMs[currentPlayer].rollDice();
    if (diceroll != 6) {
        bool pawnOnBoard = false;
        for (int i = 0; i < PAWN_AMOUNT; i++) {
            if (players[currentPlayer]->pawns[i]->isOnBoard && !players[currentPlayer]->pawns[i]->isFinished) pawnOnBoard = true;
        }
        if (!pawnOnBoard) {
            currentPlayer++;
            currentPlayer%=playerAmount;
            return false;
        }
    }

    bool AllowedToMove = true;
    do {
        delay(1000);
        int pawnToMove = UIMs[currentPlayer].selectPawn();
        AllowedToMove = true;

        // if not on board you have to roll 6 to move this pawn;
        if (!players[currentPlayer]->pawns[pawnToMove]->isOnBoard) {
            if (diceroll == 6) {
                board->movePawn(players[currentPlayer]->pawns[pawnToMove], 0); // move to first tile
                players[currentPlayer]->pawns[pawnToMove]->isOnBoard = true;
                Serial.println("pawn moved to board");
                break;
            } else {
                AllowedToMove = false;
                UIMs[currentPlayer].transmit(UIMCOMMAND_BLINK);
                Serial.println("pawn not allowed to move");
                continue;
            }
        }
        Serial.println("pawn is on board");

        for (int i = 0; i < PAWN_AMOUNT; i++) {
            if (players[currentPlayer]->pawns[i]->step == players[currentPlayer]->pawns[pawnToMove]->step + diceroll) {
                AllowedToMove = false;
                UIMs[currentPlayer].transmit(UIMCOMMAND_BLINK);
                Serial.println("pawn capturing self");
                continue;
            }
        }
        Serial.println("pawn moved");
        board->movePawn(players[currentPlayer]->pawns[pawnToMove], diceroll);
    } while(!AllowedToMove);

    //board->path->updateMap();
    //board->path->printMap();

    currentPlayer++;
    currentPlayer%=playerAmount;
    
    return true;
};
