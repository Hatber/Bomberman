#include "GameEngine.hpp"
#include "../Common.hpp"

#include "libtcod.hpp"

#include <algorithm>

GameEngine::GameEngine() :
    _currentBombBuild(BombFactory::MakeDefaultBomb()),
    _level(levelXSize, levelYSize)
{
    _findedPowers.insert(3);
    _findedTimers.insert(3);

    _hero.SetMaxHP(defaultMaxHP);
    _hero.SetCurrentHP(defaultMaxHP);

    _difficulty = 1;

    Generate();
}

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
    if(MCoordinates(_hero) == MCoordinates(_exit)) {
        _difficulty+=2;
        Generate();
        return false;
    }

    if(_level.GetType(_hero.first, _hero.second) != BOMB ) {
        Bomb newBomb(_currentBombBuild);
        newBomb.SetCoordinate(_hero);
        _bombs.push_back(newBomb);

        return true;
    }

    return false;
}

bool GameEngine::DoAction() {
    if(MCoordinates(_hero) == MCoordinates(_exit)) {
        _difficulty++;
        Generate();
    }
    return true;
}

void GameEngine::UpdateLevel() {
    _level.Clear();

    _level.SetExit(_exit);
    for(std::size_t i = 0; i < _walls.size(); i++)    { _level.SetWall(_walls[i]); }
    for(std::size_t i = 0; i < _monsters.size(); i++) {
        switch(_monsters[i].GetType()) {
            case NORMAL : _level.SetMonsterNormal(_monsters[i]); break;
            case FAST   : _level.SetMonsterFast(_monsters[i]);   break;
            case SLOW   : _level.SetMonsterSlow(_monsters[i]);   break;
        }
    }
    for(std::size_t i = 0; i < _bombs.size(); i++)    { _level.SetBomb(_bombs[i]); }
    for(std::size_t i = 0; i < _bonuses.size(); i++)  { _level.SetBonus(*_bonuses[i]); }

    _level.SetHero(_hero);
}

void GameEngine::Step(TCODMapWrapper& pathMap) {
    UpdateLevel();

    TCODPath *path = new TCODPath(&pathMap, 0.0);
    for(std::size_t monsterIt = 0; monsterIt < _monsters.size(); monsterIt++) {

        bool monsterFindedHero = path->compute(
            _monsters[monsterIt].first, _monsters[monsterIt].second,
            _hero.first, _hero.second
        );

        if(monsterFindedHero) {
            if(path->size() > _monsters[monsterIt].GetAtackRange()) {
                int newXMonsterPosition;
                int newYMonsterPosition;

                path->walk(&newXMonsterPosition, &newYMonsterPosition, false);
                _monsters[monsterIt].SetCoordinate(newXMonsterPosition, newYMonsterPosition);
            } else {
                _hero.ReduceHP(_monsters[monsterIt].GetAtackPower());
            }
        }
    }
    delete path;

    for(std::size_t i = 0; i < _bombs.size(); i++) { _bombs[i].Tic(); }

    explosionCover.clear();
    bool explosionComplete = true;
    do {
        explosionComplete = true;
        for(std::size_t i = 0; i < _bombs.size(); i++) {
            if(_bombs[i].ReadyToExplosion()) {

                explosionComplete = false;

                if(_bombs[i].ContainDirection(UP))     { ExplosionTransmission(_bombs[i], UP, _bombs[i].GetPower()); }
                if(_bombs[i].ContainDirection(DOWN))   { ExplosionTransmission(_bombs[i], DOWN, _bombs[i].GetPower()); }
                if(_bombs[i].ContainDirection(RIGHT))  { ExplosionTransmission(_bombs[i], RIGHT, _bombs[i].GetPower()); }
                if(_bombs[i].ContainDirection(LEFT))   { ExplosionTransmission(_bombs[i], LEFT, _bombs[i].GetPower()); }
                if(_bombs[i].ContainDirection(CENTER)) { ExplosionTransmission(_bombs[i], CENTER, _bombs[i].GetPower()); }

                _bombs.erase(_bombs.begin() + i);
            }
        }
    } while (!explosionComplete);
}

void GameEngine::PutToDeath() {
    _monsters.erase(std::remove_if(_monsters.begin(), _monsters.end(), Creation::Dead),
              _monsters.end());
    _walls.erase(std::remove_if(_walls.begin(), _walls.end(), Creation::Dead),
              _walls.end());
}

void GameEngine::Generate() {
    _walls.clear();
    _monsters.clear();
    _bonuses.clear();
    _level.FullClear();

    _hero.SetCoordinate(0, 0);
    _hero.Heal();

    TCODRandom * rnd = TCODRandom::getInstance();
    rnd->setDistribution(TCOD_DISTRIBUTION_LINEAR);

    MCoordinates currentPoint;
    for(int i = 0; i < _level.GetYSize(); i++) {
        for(int j = 0; j < _level.GetXSize(); j++) {
            currentPoint.SetCoordinate(j, i);
            if(!currentPoint.InBorders(MCoordinates(0, 0), MCoordinates(3, 3)) && rnd->getInt(0, 3) == 0) {
               DestructibleObject wall(MCoordinates(j, i));
               wall.SetMaxHP(1);
               wall.SetCurrentHP(1);
                _walls.push_back(wall);
            }
        }
    }
    UpdateLevel();

    for(int i = 0; i < _difficulty;) {
        int xCoord = rnd->getInt(10, levelXSize - 1);
        int yCoord = rnd->getInt(10, levelYSize - 1);

        if(_level.IsWalkable(xCoord, yCoord)) {
            MonsterT typeM = NORMAL;
            switch(rnd->getInt(0, 3)) {
                case 0: typeM = NORMAL; break;
                case 1: typeM = FAST;   break;
                case 2: typeM = SLOW;   break;
            }
            Monster newMonster(MCoordinates(xCoord, yCoord), typeM);
            newMonster.SetMaxHP(1);
            newMonster.SetCurrentHP(1);

            _monsters.push_back( newMonster );

            i++;
        }
    }
    UpdateLevel();


    int xFreeCoord;
    int yFreeCoord;
    do {
        xFreeCoord = rnd->getInt(10, levelXSize - 1);
        yFreeCoord = rnd->getInt(10, levelYSize - 1);
    } while(!_level.IsWalkable(xFreeCoord, yFreeCoord));

    _exit.SetCoordinate(xFreeCoord, yFreeCoord);

    UpdateLevel();
}

void GameEngine::ExplosionTransmission(MCoordinates current, DirectionT diretion, int power) {

    if(power < 1) { return; }
    explosionCover.insert(current);
    //Проверяем на столкновение
    MCoordinates offsetCoordinate;
    switch(diretion) {
        case UP     : offsetCoordinate = current + MCoordinates(0, -1); break;
        case DOWN   : offsetCoordinate = current + MCoordinates(0, 1); break;
        case LEFT   : offsetCoordinate = current + MCoordinates(-1, 0); break;
        case RIGHT  : offsetCoordinate = current + MCoordinates(1, 0); break;
        case CENTER : offsetCoordinate = current + MCoordinates(0, 0); break;
    }

    bool explosionInLevel = offsetCoordinate.InBorders(MCoordinates(0, 0), MCoordinates(levelXSize, levelYSize));

    //BOMB COLLIDE
    for(std::size_t i = 0; i < _bombs.size(); i++) {
        if(MCoordinates(_bombs[i]) == offsetCoordinate) {
            _bombs[i].Reset();
        }
    }

    //HERO COLLIDE
    if(MCoordinates(_hero) == offsetCoordinate) {
        _hero.ReduceHP(power);
    }

    //MONSTERS COLLIDE
    for(std::size_t i = 0; i < _monsters.size(); i++) {
        if(MCoordinates(_monsters[i]) == offsetCoordinate) {
            _monsters[i].ReduceHP(power);
        }
    }

    //WALLS COLLIDE
    for(std::size_t i = 0; i < _walls.size(); i++) {
        if(MCoordinates(_walls[i]) == offsetCoordinate) {
            _walls[i].ReduceHP(power);
        }
    }

    if( (!_level.IsWalkable(current.first, current.second) || _level.GetType(current.first, current.second)==HERO)
        && _level.GetType(current.first, current.second)!=BOMB) {
        power--;
    }

    if(explosionInLevel) {
        ExplosionTransmission(offsetCoordinate, diretion, power - 1);
    }

    return;
}

bool GameEngine::IsDestructible(int x, int y) {
    MCoordinates coord(x, y);

    return true;
}

bool GameEngine::IsLevelBorder(int x, int y) {
    return true;
}
