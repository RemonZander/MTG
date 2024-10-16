#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "config.h"

#include <stdint.h>

typedef struct Cordinates_s {
    uint8_t x;
    uint8_t y;
} Coordinates_t;

typedef struct BoardMap_s {
    Coordinates_t size;
    uint8_t map[BOARD_SIZE_X_MAX][BOARD_SIZE_Y_MAX];
} BoardMap_t;

#endif
