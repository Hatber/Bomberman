#ifndef __MONSTER__
#define __MONSTER__

#include "Creation.hpp"

const float defaultAtackPower = 2.49;
const int defaultAtackRange = 2;

enum MonsterT { NORMAL, FAST, SLOW };

class Monster : public Creation {
public:
    Monster(MonsterT type = NORMAL, float atackPower = defaultAtackPower, int atackRange = defaultAtackRange)
        : _type(type), _atackPower(atackPower), _atackRange(atackRange) { }
    Monster(const MCoordinates& coord) : Creation(coord),
        _type(NORMAL), _atackPower(defaultAtackPower), _atackRange(defaultAtackRange) { }

    Monster(const MCoordinates& coord, MonsterT type) : Creation(coord),
        _type(type), _atackPower(defaultAtackPower), _atackRange(defaultAtackRange) { }

    float GetAtackPower() { return _atackPower / GetMultipler(); }
    float GetAtackRange() { return _atackRange * GetMultipler(); }
    MonsterT GetType()    { return _type; }

private:
    float GetMultipler() {
        switch(_type) {
            case NORMAL : return 1;
            case FAST   : return 1.5;
            case SLOW   : return 0.5;
        }

        return 1;
    }
    MonsterT _type;
    float _atackPower;
    int _atackRange;
};

#endif // __MONSTER__
