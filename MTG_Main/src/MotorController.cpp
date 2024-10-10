#include "MoterController.hpp"
#include <math.h>

#define BUILTIN_LED 13
#define stepPinB 8
#define dirPinB 9
#define limitY 10

#define stepPinA 5
#define dirPinA 6
#define limitX 7

#define MAX_SPEED 1000
#define NORMAL_SPEED 1000
#define SLOW_SPEED 1000

#define SWITCH_OFFSETX 60
#define SWITCH_OFFSETY 105

//van hart vak links naar hart vak rechts van speelbord is 225mm. Stap is dus 0.1mm
#define MAXBOARD_SIZEX 2250
#define MAXBOARD_SIZEY 2250

#define electromagnetPin 2

MoterController::MoterController()
{
    stepperMotorA = new AccelStepper(1, stepPinA, dirPinA);
    stepperMotorB = new AccelStepper(1, stepPinB, dirPinB);

    stepperMotorB->setMaxSpeed(MAX_SPEED);

    stepperMotorB->setSpeed(NORMAL_SPEED);

    stepperMotorA->setMaxSpeed(MAX_SPEED);
    stepperMotorA->setAcceleration(1000);
    stepperMotorA->setSpeed(NORMAL_SPEED);
    pinMode(electromagnetPin, OUTPUT);

    magnetOff();
};

MoterController::~MoterController()
{
}

int MoterController::getMaxBoardX(){return MAXBOARD_SIZEX;};
int MoterController::getMaxBoardY(){return MAXBOARD_SIZEY;};



void MoterController::HomeMotors()
{
    
    // Set offset to dont break the switch
    this->currentX = 0;
    this->currentY = 0;
    delay(100);
    MotorToPos(SWITCH_OFFSETX, SWITCH_OFFSETY );
    this->currentX = 0;
    this->currentY = 0;

    stepperMotorA->setSpeed(SLOW_SPEED);
    stepperMotorB->setSpeed(-SLOW_SPEED);



    while (digitalRead(limitY))
    {
        stepperMotorA->runSpeed();
        stepperMotorB->runSpeed();
    }

    stepperMotorB->setSpeed(SLOW_SPEED);
    while (digitalRead(limitX))
    {
        stepperMotorA->runSpeed();
        stepperMotorB->runSpeed();
    }

    // Set offset to dont break the switch
    this->currentX = 0;
    this->currentY = 0;
    delay(100);
    MotorToPos(SWITCH_OFFSETX, SWITCH_OFFSETY);
    this->currentX = 0;
    this->currentY = 0;
    delay(1000);

}

void MoterController::test()
{
    MotorToPos(0, MAXBOARD_SIZEY);
    MotorToPos(MAXBOARD_SIZEX, MAXBOARD_SIZEY);
    MotorToPos(MAXBOARD_SIZEX, 0);
    MotorToPos(0,0);

}

void MoterController::magnetOn(){
    digitalWrite(electromagnetPin, this->isMagnetOn = true);
    digitalWrite(BUILTIN_LED, this->isMagnetOn = true);
 
}

void MoterController::magnetOff(){
    digitalWrite(electromagnetPin, this->isMagnetOn = false);
 digitalWrite(BUILTIN_LED, this->isMagnetOn = false);
}

bool MoterController::MotorToPos(int x, int y)
{
    if(x >MAXBOARD_SIZEX || y > MAXBOARD_SIZEY )
        return false;

    int deltaA =  (y - this->currentY) + (x - this->currentX);
    int deltaB =  (y - this->currentY) - (x - this->currentX);

    

    int dirA = deltaA < 0 ? 1 : -1;
    int dirB = deltaB > 0 ? 1 : -1;
    stepperMotorA->setSpeed(NORMAL_SPEED * dirA);
    stepperMotorB->setSpeed(NORMAL_SPEED * dirB);

    for (int i = 0; i < max(abs(deltaA), abs(deltaB)); i++)
    {

        if (i < abs(deltaA))
        {
            while (!stepperMotorA->runSpeed())
                ;
        }
        if (i < abs(deltaB))
        {
            while (!stepperMotorB->runSpeed())
                ;
        }
    }
    this->currentX = x;
    this->currentY = y;

    return true;
}
