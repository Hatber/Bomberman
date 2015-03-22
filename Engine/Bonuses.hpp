#ifndef __BONUSES__
#define __BONUSES__

#include "Mixins/MCoordinates.hpp"

enum DirectionT { UP, RIGHT, DOWN, LEFT, CENTER };
enum BonusT{TIMER_BONUS, POWER_BONUS, DIRECTION_BONUS};

class IBonus : public MCoordinates {
public:
    IBonus(BonusT type) : _type(type) { }
    BonusT GetBonusType() { return _type; }

private:
    const BonusT _type;
};

template < typename T >
class Bonus : public IBonus {
public:
    Bonus(T value, BonusT type) : IBonus(type), _value(value) {}
    T GetValue() { return _value; };

private:
    const T _value;
};

class BonusFactory {
public:
    static IBonus* GetTimerBonus(int timer) { return new Bonus<int>(timer, TIMER_BONUS); }
    static IBonus* GetPowerBonus(float power) { return new Bonus<float>(power, POWER_BONUS); }
    static IBonus* GetDirectionBonus(DirectionT direction){ return new Bonus<DirectionT>(direction, DIRECTION_BONUS); }
};

#endif // __BONUSES__
