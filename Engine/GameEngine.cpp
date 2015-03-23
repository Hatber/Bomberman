#include "GameEngine.hpp"
#include "../Common.hpp"

#include "libtcod.hpp"

GameEngine::GameEngine() :
    _level(levelXSize, levelYSize),
    _currentBombBuild(BombFactory::MakeDefaultBomb())
{
    Generate();
    UpdateLevel();
}

#include <iostream>
using std::cout;
using std::endl;

bool GameEngine::MoveHero(int xOffset, int yOffset) {
    MCoordinates heroOffset(MCoordinates(_hero) + MCoordinates(xOffset, yOffset));


    bool heroInLevel = heroOffset.InBorders(MCoordinates(0, 0), MCoordinates(levelXSize, levelYSize));
    if(heroInLevel && _level.IsWalkable(heroOffset.first, heroOffset.second)) {
        _hero.SetCoordinate(heroOffset);
        return true;
    }

    return false;
}

bool GameEngine::PlaceBomb() {
    if(_level.GetType(_hero.first, _hero.second) != BOMB ) {
        Bomb newBomb(_currentBombBuild);
        newBomb.SetCoordinate(_hero);
        _bombs.push_back(newBomb);

        return true;
    }

    return false;
}

bool GameEngine::DoAction() {
    return true;
}

void GameEngine::UpdateLevel() {
    _level.Clear();

    _level.SetExit(_exit);
    for(std::size_t i = 0; i < _walls.size(); i++)    { _level.SetWall(_walls[i]); }
    for(std::size_t i = 0; i < _monsters.size(); i++) { _level.SetMonster(_monsters[i]); }
    for(std::size_t i = 0; i < _bombs.size(); i++)    { _level.SetBomb(_bombs[i]); }
    for(std::size_t i = 0; i < _bonuses.size(); i++)  { _level.SetBonus(*_bonuses[i]); }

    _level.SetHero(_hero);
}

void GameEngine::Generate() {
    _walls.clear();
    _monsters.clear();
    _bonuses.clear();

    _hero.SetCoordinate(0, 0);


    TCODRandom * rnd = TCODRandom::getInstance();
    rnd->setDistribution(TCOD_DISTRIBUTION_LINEAR);

    MCoordinates currentPoint;
    for(int i = 0; i < _level.GetYSize(); i++) {
        for(int j = 0; j < _level.GetXSize(); j++) {
            currentPoint.SetCoordinate(j, i);
            if(!currentPoint.InBorders(MCoordinates(0, 0), MCoordinates(3, 3)) && rnd->getInt(0, 2) == 0) {
               DestructibleObject wall(MCoordinates(j, i));
                _walls.push_back(wall);
            }
        }
    }

    for(int i = 0; i < 5;) {
        int xCoord = rnd->getInt(10, levelXSize - 1);
        int yCoord = rnd->getInt(10, levelYSize - 1);

        if(_level.IsWalkable(xCoord, yCoord)) {
            _monsters.push_back(Monster(MCoordinates(xCoord, yCoord)));
            i++;
        }
    }

    _exit.SetCoordinate(rnd->getInt(10, levelXSize - 1), rnd->getInt(10, levelYSize - 1));

    delete rnd;
}
