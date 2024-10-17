#include <Pawns.hpp>
#include <vector>

template <class T, class U>
class Player {
    public:
        Player(uint8_t ID, std::vector<Pawn<U>>* Pawns,T State){
            this->ID = ID;
            this->Pawns = Pawns;
            this->State = State;
        }

        uint8_t ID;
        std::vector<Pawn<U>>* Pawns;
        T State;
};