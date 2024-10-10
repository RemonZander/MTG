#ifndef UIMDICELED_H
#define UIMDICELED_H

enum uimDiceLed {
    UIMDICE_1_LED_1 = 7,
    UIMDICE_1_LED_2 = 6,
    UIMDICE_1_LED_3 = 5,
    UIMDICE_1_LED_4 = 4,

    UIMDICE_2_LED_1 = 3,
    UIMDICE_2_LED_2 = 2,
    UIMDICE_2_LED_3 = 1,
    UIMDICE_2_LED_4 = 0
};

enum uimShiftPins {
    UIMSHIFT_LATCH = 3,
    UIMSHIFT_DATA = 2,
    UIMSHIFT_CLOCK = 4
};

#endif