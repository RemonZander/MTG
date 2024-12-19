#include "LudoGame.hpp"

TEST(LudoGame, DOTUNRS)
{
    LudoGame *game = new LudoGame();
    game->Init();
    game->GameLoop();
    delete(game);
}