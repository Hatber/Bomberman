#ifndef __BOMB__
#define __BOMB__

#include "Bonuses.hpp"

#include <set>

class Bomb {
public:
    Bomb(float power, int timer) : _power(power), _timer(timer) { }

    void Tic() { _timer--; }
    bool ReadyToExplosion() { return _timer == 0; }

    void AddDirection(DirectionT direction) { _directions.insert(direction); }
    void RemoveDirection(DirectionT direction) { _directions.erase(direction); }

    bool ContainDirection(DirectionT direction) { return _directions.find(direction) != _directions.end(); }

    float GetPower() { return _power; }
    int  GetTimer()  { return _timer; }

private:
    float _power;
    int _timer;

    std::set< DirectionT > _directions;
};

#endif // __BOMB__
