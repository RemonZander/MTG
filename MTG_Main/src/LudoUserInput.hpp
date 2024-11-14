#ifndef LUDOUSERINPUT_HPP
#define LUDOUSERINPUT_HPP
#include "UserInputModule.hpp"

class LudoInputModule: public UserInputModule {
    public:
        LudoInputModule(int slaveAddr): UserInputModule(slaveAddr){};
        int rollDice();
        int selectPawn();
};

#endif