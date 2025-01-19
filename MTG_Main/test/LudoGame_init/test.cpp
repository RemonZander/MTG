#include <gtest/gtest.h>
#include "./LudoGame.hpp"
#include "../../main/logger.h"
#include <stdint.h>
#include <stdio.h>

#define GTEST_COUT std::cerr << "[          ] [ INFO ]"


TEST(LudoGame, init)
{
    LudoGame *game = new LudoGame();
    game->Init();
    delete(game);
}