#include <gtest/gtest.h>
#include "LudoGame.hpp"

TEST(LudoGame, init)
{
    LudoGame *game = new LudoGame();
    game->Init();
    game->GameLoop();
    delete(game);
}