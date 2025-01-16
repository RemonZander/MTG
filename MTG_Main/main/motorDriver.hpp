#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <stdint.h>
#include "typedefs.h"

#ifdef ARDUINO
#include <AccelStepper.h>
#endif

#ifdef IDF_VER
#include <soc/gpio_num.h>
#endif
struct motorPins_t {
    gpio_num_t step;
    gpio_num_t dir;
};

class MotorDriver {
public:
    MotorDriver();
    ~MotorDriver();

    /** MotorDriver::init()
     * initilizes the hardware modules
     */
    int init();

    /** move
     * move the motors relitivly
     * 
     * args:
     * - x: distance to move in mm in the x direction
     * - y: distance to move in mm in the y direction
     */
    void move(float x, float y);

    /** home
     * Move to endstops (home position)
     * 
     * args:
     * - offsetX: x axes distande in mm from home to coordinats (0, 0)
     * - offsetY: y axes distande in mm from home to coordinats (0, 0)
     */
    void home(float offsetX, float offsetY);

    /** setMagnet
     * enable or disable electromagnet
     * 
     * args:
     * - state: true: magnet on, false: magnet off
     */
    void setMagnet(bool state);
};

#endif
