#ifndef UIMSLAVE_H
#define UIMSLAVE_H

class UimSlave {  
    public:
        void begin(const int address, void (*parseFunction)(int), int *state);
        static void requestEvent();
        static void receiveEvent(int bytes);
};
#endif