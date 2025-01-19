#include "LudoBoard.hpp"
#define FINISH_LANE_LENGTH 6
#define LAP_LENGTH 52
#define QUART_LAP LAP_LENGTH / 4
#define STEPS_TO_FINISH LAP_LENGTH + FINISH_LANE_LENGTH - 2

const int posXTable[] PROGMEM = {0, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 7, 7, 7, 6, 5, 4, 3, 2, 1, 1, 1, 1, 1, 1, 0, -1, -1, -1, -1, -1, -1, -2, -3, -4, -5, -6, -7, -7, -7, -6, -5, -4, -3, -2, -1, -1, -1, -1, -1, -1, 0};

void LudoBoard::movePawn(Pawn *pawn, int steps)
{
    int step = pawn->step;

    // not allowed to move when destination is outside of finish land or of it is one tile in front of the finish, to prevent soft lock
    if (step + steps > pawn->beginStep + STEPS_TO_FINISH || step + steps == pawn->beginStep + STEPS_TO_FINISH - 1) return;
    
    for (int currentStep = 0; currentStep <= steps; currentStep++)
    {
        int x = 0, y = 0;
        if (step+currentStep <= LAP_LENGTH+pawn->beginStep-2) {
            x = stepToXPos(step + currentStep);
            y = stepToYPos(step + currentStep);
        } else {
            Serial.println("on finish lane!");
            x = stepToXPos(pawn->beginStep-2) - (step+currentStep - (LAP_LENGTH+pawn->beginStep-2)) * (int)round(sin(PI/2*floor(pawn->id/10)));
			y = stepToYPos(pawn->beginStep-2) - (step+currentStep - (LAP_LENGTH+pawn->beginStep-2)) * (int)round(cos(PI/2*floor(pawn->id/10)));
            Serial.println("Moving to");
            Serial.print(x);
            Serial.print("|");
            Serial.print(y);
        }

        int *instructions = nullptr;
        int instructionSize;

        if (!path->find(pawn->x, pawn->y, x, y, &instructions, &instructionSize))
        {
            if (currentStep == steps) {
                capturePawn(x, y);
                path->find(pawn->x, pawn->y, x, y, &instructions, &instructionSize);
            }
            else continue;
        }

        for (int i = 0; i < instructionSize; i++)
        {
            int instructionX = instructions[i * 2];
            int instructionY = instructions[i * 2 + 1];
            GameBoard::movePawn(pawn->x, pawn->y, instructionX, instructionY);
            pawn->x = instructionX;
            pawn->y = instructionY;
            //delay(100);
        }


        if (instructions != nullptr) delete(instructions);
    }
    pawn->step += steps;
    if (!pawn->isFinished && pawn->step == LAP_LENGTH+FINISH_LANE_LENGTH+pawn->beginStep-2) {
        pawn->isFinished = true;
        delay(1000);
        movePawn(pawn, pawn->HomeX-1, pawn->HomeY);
    }
}

int LudoBoard::stepToXPos(int step)
{
    if (step < 0) step = 52 + step%52;
    step %= 52;
    step *= 1;
    int pos = pgm_read_word_near(posXTable + step);
    return pos;
}

int LudoBoard::stepToYPos(int step)
{
    return stepToXPos(step + FREQ); // fase shift with the frequency
}


void LudoBoard::capturePawn(int x, int y) {
    Pawn* pawnToCapture = nullptr;
    
    for (int i = 0; i < pawnAmount; i++) {
        if (pawns[i]->x == x && pawns[i]->y == y) pawnToCapture = pawns[i];
    }

    if(pawnToCapture == nullptr)
        return;

    Serial.print("Pawn capturing: ");
    Serial.print(pawnToCapture->id);
    Serial.print("\t ");

    Serial.print(pawnToCapture->HomeX);
    Serial.print("|");

    Serial.println(pawnToCapture->HomeY);



    int *instructions = nullptr;
    int instructionSize;

    if (!path->find(pawnToCapture->x, pawnToCapture->y, pawnToCapture->HomeX, pawnToCapture->HomeY, &instructions, &instructionSize))
        path->find(pawnToCapture->x, pawnToCapture->y, 0, 0, &instructions, &instructionSize);


    for (int i = 0; i < instructionSize; i++){
        GameBoard::movePawn(pawnToCapture, instructions[i * 2], instructions[i * 2 + 1]);
    }

    if (instructions != nullptr) delete(instructions);

    pawnToCapture->step = pawnToCapture->beginStep;
    pawnToCapture->isOnBoard = false;
}