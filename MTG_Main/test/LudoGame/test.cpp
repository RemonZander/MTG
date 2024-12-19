#include <gtest/gtest.h>
#include "LudoGame.hpp"
#include "../../main/logger.h"
#include <stdint.h>
#include <stdio.h>


TEST(LudoGame, init)
{
    printf("initializing test\n");
    LudoGame *game = new LudoGame();
    printf("created game object\n");
    game->Init();
    game->GameLoop();
    delete(game);
}