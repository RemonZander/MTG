#ifndef PATH_H
#define PATH_H

#include <cstdint>
#include <stdint.h>
#include <stdbool.h>
#include <vector>

#include "config.h"
#include "typedefs.h"

class Path
{
public:
    Path();
    ~Path();

    bool Execute();
    void AddStep();
    void SetSteps();
    uint16_t GetStepNum();
    uint16_t GetLength();

private:
    std::vector<Coordinates_t>* _steps;
};

#endif
