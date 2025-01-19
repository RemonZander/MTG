#ifndef LUDOINPUTMODULE_HPP
#define LUDOINPUTMODULE_HPP

#include <cstdint>
#include <stdio.h>
#include "../../main/logger.h"

class LudoInputModule {
    public:
        LudoInputModule(uint8_t slaveAddr);
        int rollDice();
        int selectPawn();
    
    private:
        uint8_t slaveAddr;
};


#endif