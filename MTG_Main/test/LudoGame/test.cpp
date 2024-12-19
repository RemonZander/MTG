//#include <gtest/gtest.h>
#include "LudoGame.hpp"
#include "../../main/logger.h"
#include <stdint.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    printf("initializing test");
    LudoGame *game = new LudoGame();
    printf("created game object");
    game->Init();
    game->GameLoop();
    delete(game);
    return 0;
}


// TEST(LudoGame, init)
// {
//     printf("initializing test");
//     LudoGame *game = new LudoGame();
//     printf("created game object");
//     game->Init();
//     game->GameLoop();
//     delete(game);
// }