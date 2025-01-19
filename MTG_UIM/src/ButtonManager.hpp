#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H

#include "UIMButton.h"

class ButtonManager {  
    public:
        ButtonManager();
        uimButton buttonPressed();
        void toggleLed(uimButtonLED led, bool state);
};
#endif
