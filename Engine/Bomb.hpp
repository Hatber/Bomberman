#ifndef __BOMB__
#define __BOMB__

#include "Bonuses.hpp"
#include "Mixins/MCoordinates.hpp"

#include "libtcod.hpp"

#include <set>

class Bomb : public MCoordinates {
public:
    Bomb(float power, int timer) : _power(power), _timer(timer) { }

    void Tic() { _timer--; }
    void Reset() { _timer = 0; }
    bool ReadyToExplosion() { return _timer == 0; }

    void AddDirection(DirectionT direction) { _directions.insert(direction); }
    void RemoveDirection(DirectionT direction) { _directions.erase(direction); }

    bool ContainDirection(DirectionT direction) { return _directions.find(direction) != _directions.end(); }

    void SetPower(float power) { _power = power; }
    void SetTimer(int timer)   {  _timer = timer; }

    float GetPower() { return _power; }
    int   GetTimer()  { return _timer; }

private:
    float _power;
    int _timer;

    std::set< DirectionT > _directions;
};

class BombFactory {
public:
    static Bomb MakeDefaultBomb() {
        Bomb defaultBomb(5, 3);
        defaultBomb.AddDirection(UP);
        defaultBomb.AddDirection(RIGHT);
        defaultBomb.AddDirection(LEFT);
        defaultBomb.AddDirection(DOWN);
        defaultBomb.AddDirection(CENTER);

        return defaultBomb;
    }

    static void Modify(Bomb& bomb) {
        TCODRandom * rnd = TCODRandom::getInstance();

        int timerModif = rnd->getInt(0,2) - 1;
        int powerModif = rnd->getInt(0,2) - 1;

        if(bomb.GetTimer() + timerModif > 3 && bomb.GetTimer() + timerModif <= 9) { bomb.SetTimer(bomb.GetTimer() + timerModif); }
        if(bomb.GetPower() + powerModif > 3 && bomb.GetPower() + powerModif <= 9) { bomb.SetPower(bomb.GetPower() + powerModif); }

        bomb.AddDirection(UP);
        bomb.AddDirection(RIGHT);
        bomb.AddDirection(LEFT);
        bomb.AddDirection(DOWN);
        bomb.AddDirection(CENTER);

        DirectionT rndDirection = UP;
        switch(rnd->getInt(0,4)) {
            case 0: rndDirection = UP; break;
            case 1: rndDirection = DOWN; break;
            case 2: rndDirection = LEFT; break;
            case 3: rndDirection = RIGHT; break;
            case 4: rndDirection = CENTER; break;
        }

        bomb.RemoveDirection(rndDirection);
    }
};

#endif // __BOMB__
