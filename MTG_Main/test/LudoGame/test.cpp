#include <gtest/gtest.h>
#include "LudoGame.hpp"
#include "../../main/logger.h"
#include <stdint.h>
#include <stdio.h>

#define GTEST_COUT std::cerr << "[          ] [ INFO ]"


TEST(LudoGame, init)
{
    GTEST_COUT << "initializing test" << std::endl;
    LudoGame *game = new LudoGame();
    GTEST_COUT << "created game object" << std::endl;
    game->Init();
    game->GameLoop();
    delete(game);
}