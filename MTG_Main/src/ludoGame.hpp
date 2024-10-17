#include <GameBase.hpp>

class LudoGame : public GameBase<int, int> {
    public:
        void GameLoop();
        void Init();
};