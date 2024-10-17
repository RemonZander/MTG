#include <stdint.h>
#include <Player.hpp>

typedef struct BordSize_s {
    Cordinates_s size;
    Cordinates_s squareSize;
} BordSize_t;

template <class T, class U>
class GameBase {
    public:
        virtual void GameLoop();
        virtual void Init();
    
    protected:
        std::vector<Player<T, U>>* players;
        BoardMap_t map;
        BordSize_t bordSize;
};