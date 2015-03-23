#ifndef __GAME_ENGINE__
#define __GAME_ENGINE__

#include "Level/Level.hpp"
#include "Creation.hpp"
#include "Monster.hpp"
#include "Bomb.hpp"
#include "Bonuses.hpp"

#include <vector>
#include <set>

typedef Creation DestructibleObject;

class GameEngine {
public:
    GameEngine();

    bool MoveHero(int xOffset, int yOffset);
    bool PlaceBomb();
    bool DoAction();
private:
    void UpdateLevel();

    Level _level;
    Creation _hero;
    std::vector< DestructibleObject > _walls;
    std::vector< Monster > _monsters;
    std::vector< Bomb > _bombs;
    std::vector< IBonus* > _bonuses;
    MCoordinates _exit;

    Bomb _currentBombBuild;

    std::set< int > _findedTimers;
    std::set< int > _findedPowers;
    std::set< DirectionT > _findedDirections;
};

#endif // __GAME_ENGINE__

