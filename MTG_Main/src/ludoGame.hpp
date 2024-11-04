#include <GameBase.hpp>
#include <LudoUserInput.hpp>
#include <typedefs.h>

class LudoGame : public GameBase<LudoInputModule, int, LudoGameStates> {
    public:
        void GameLoop();
        void Init();
};