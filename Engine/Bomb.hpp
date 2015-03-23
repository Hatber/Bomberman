#ifndef __BOMB__
#define __BOMB__

#include "Bonuses.hpp"
#include "Mixins/MCoordinates.hpp"

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

    float GetPower() { return _power; }
    int  GetTimer()  { return _timer; }

private:
    float _power;
    int _timer;

    std::set< DirectionT > _directions;
};

class BombFactory {
public:
    static Bomb MakeDefaultBomb() {
        Bomb defaultBomb(4, 3);
        defaultBomb.AddDirection(UP);
        defaultBomb.AddDirection(RIGHT);
        defaultBomb.AddDirection(LEFT);
        defaultBomb.AddDirection(DOWN);
        defaultBomb.AddDirection(CENTER);

        return defaultBomb;
    }
};

#endif // __BOMB__
