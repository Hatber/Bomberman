#ifndef __MONSTER__
#define __MONSTER__

#include "Creation.hpp"

const float defaultAtackPower = 2.49;
const int defaultAtackRange = 1;

enum MonsterT { NORMAL, FAST, SLOW };

class Monster : public Creation {
public:
    Monster(float atackPower = defaultAtackPower, int atackRange = defaultAtackRange, MonsterT type = NORMAL)
        : _atackPower(atackPower), _atackRange(atackRange), _type(type) { }

    float getAtackPower() { return _atackPower; }
    float getAtackRange() { return _atackRange; }

private:
    float _atackPower;
    int _atackRange;

    MonsterT _type;
};

#endif // __MONSTER__
