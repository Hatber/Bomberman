#ifndef __GAME_ENGINE__
#define __GAME_ENGINE__

#include "../Util/TCODMapWrapper.hpp"
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

    Level& GetLevel() { return _level; }

    MHitPoints< float > GetHeroHP() { return _hero; }
    MCoordinates GetHeroPosition() { return _hero; }

    void UpdateLevel();
    void Step(TCODMapWrapper& pathMap);
    void PutToDeath();

    std::set< int > _findedTimers;
    std::set< int > _findedPowers;
    std::set< DirectionT > _findedDirections;

    Bomb _currentBombBuild;

    std::set< MCoordinates > explosionCover;
    int _difficulty;

private:
    void Generate();


    void ExplosionTransmission(MCoordinates current, DirectionT diretion, int power);

    bool IsDestructible(int x, int y);
    bool IsLevelBorder(int x, int y);

    Level _level;
    Creation _hero;
    std::vector< DestructibleObject > _walls;
    std::vector< Monster > _monsters;
    std::vector< Bomb > _bombs;
    std::vector< IBonus* > _bonuses;
    MCoordinates _exit;

    int _score;
};

#endif // __GAME_ENGINE__

