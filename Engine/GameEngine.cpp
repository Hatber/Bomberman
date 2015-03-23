#include "GameEngine.hpp"
#include "../Common.hpp"

GameEngine::GameEngine() :
    _level(levelXSize, levelYSize),
    _currentBombBuild(BombFactory::MakeDefaultBomb())
{ }

bool GameEngine::MoveHero(int xOffset, int yOffset) {
    MCoordinates heroOffset(MCoordinates(_hero) + MCoordinates(xOffset, yOffset));

    bool heroInLevel = heroOffset.InBorders(MCoordinates(0, levelXSize), MCoordinates(0, levelYSize));
    bool heroCanMovable = _level.IsWalkable(heroOffset.first, heroOffset.second);
    if(heroInLevel && heroCanMovable) {
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

    _level.SetHero(_hero);
    _level.SetExit(_exit);
    for(std::size_t i = 0; i < _walls.size(); i++)    { _level.SetWall(_walls[i]); }
    for(std::size_t i = 0; i < _monsters.size(); i++) { _level.SetMonster(_monsters[i]); }
    for(std::size_t i = 0; i < _bombs.size(); i++)    { _level.SetBomb(_bombs[i]); }
    for(std::size_t i = 0; i < _bonuses.size(); i++)  { _level.SetBonus(*_bonuses[i]); }
}
