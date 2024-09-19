#include <AccelStepper.h>


class MoterController
{
private:
    AccelStepper *stepperMotorA = 0;
    AccelStepper *stepperMotorB = 0;
    /* data */
public:
    int height, width;
    int currentX, currentY;
    bool isMagnetOn = false;


    MoterController();
    ~MoterController();

    void HomeMotors();

    bool MotorToPos(int x, int y);
    void test();
    void magnetOn();
    void magnetOff();
    int getMaxBoardX();
    int getMaxBoardY();
};
