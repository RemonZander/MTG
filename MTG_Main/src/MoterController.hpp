#include <cstdint>
#include <stdint.h>

#include <AccelStepper.h>

struct motorPins_t {
    uint16_t step;
    uint16_t dir;
};

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


    MoterController(motorPins_t pinsMotorA, motorPins_t pinsMotorB);
    ~MoterController();

    void HomeMotors();

    bool MotorToPos(int x, int y);
    void test();
    void magnetOn();
    void magnetOff();
};
