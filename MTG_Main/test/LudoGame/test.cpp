#include <gtest/gtest.h>
#include "LudoGame.hpp"
#include "../../main/logger.h"

TEST(LudoGame, init)
{
    LOG_I("initializing test");
    LudoGame *game = new LudoGame();
    LOG_I("created game object");
    game->Init();
    game->GameLoop();
    delete(game);
}