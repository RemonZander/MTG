#ifndef USERINPUTMODULE_HPP
#define USERINPUTMODULE_HPP
#include "UimCommand.h"

class UserInputModule {
    public:
        static bool wireStarted;
        UserInputModule(int slaveAddr);
        int requestState();
        void transmit(int value);

    private:
        int slaveAddr;
};
#endif