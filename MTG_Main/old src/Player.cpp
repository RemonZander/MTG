 #include "Player.hpp"
 #include "Arduino.h"

Player::Player(int playerNumber) {

            this->playerNumber = playerNumber;
            Serial.println("new player");
            // Serial.println(this->playerNumber);
            

            int PawnHomeCoordsX[] = {6, 6, 4, 4};
            int PawnHomeCoordsY[] = {6, 4, 4, 6};

            // create start and finish coördinates
            for (int i = 0; i < PAWN_AMOUNT; i++) {
                int x = PawnHomeCoordsX[i]*round(cos(-PI/2.0*(double)playerNumber)) - PawnHomeCoordsY[i]*round(sin(-PI/2.0*(double)playerNumber));
                int y = PawnHomeCoordsX[i]*round(sin(-PI/2.0*(double)playerNumber)) + PawnHomeCoordsY[i]*round(cos(-PI/2.0*(double)playerNumber));
                pawns[i] =  new Pawn(x, y, playerNumber*10+i);
                // Serial.print(x);
                // Serial.print(", ");
                // Serial.println(y);
                pawns[i]->step += 13*playerNumber;
                pawns[i]->beginStep = pawns[i]->step;
            }
            finish = 13*playerNumber+52;
        }