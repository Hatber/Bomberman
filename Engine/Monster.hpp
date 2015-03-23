#ifndef __MONSTER__
#define __MONSTER__

#include "Creation.hpp"

const float defaultAtackPower = 2.49;
const int defaultAtackRange = 1;

enum MonsterT { NORMAL, FAST, SLOW };

class Monster : public Creation {
public:
    Monster(MonsterT type = NORMAL, float atackPower = defaultAtackPower, int atackRange = defaultAtackRange)
        : _type(type), _atackPower(atackPower), _atackRange(atackRange) { }
    Monster(const MCoordinates& coord) : Creation(coord),
        _type(NORMAL), _atackPower(defaultAtackPower), _atackRange(defaultAtackRange) { }

    float getAtackPower() { return _atackPower; }
    float getAtackRange() { return _atackRange; }

private:
    MonsterT _type;
    float _atackPower;
    int _atackRange;
};

#endif // __MONSTER__
