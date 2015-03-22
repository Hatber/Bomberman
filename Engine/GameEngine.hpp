#ifndef __GAME_ENGINE__
#define __GAME_ENGINE__

#include "Level/Level.hpp"
#include "Creation.hpp"
#include "Monster.hpp"
#include "Bomb.hpp"
#include "Bonuses.hpp"

#include <vector>

class GameEngine {
public:
    GameEngine();

    bool MoveHero(int xOffset, int yOffset);
    bool PlaceBomb();
    bool DoAction();
private:
    Level _level;
    Creation _hero;
    std::vector< Monster > _monsters;
    std::vector< Bomb > _bombs;
    std::vector< IBonus* > _bonuses;
    MCoordinates exit;
};

#endif // __GAME_ENGINE__

