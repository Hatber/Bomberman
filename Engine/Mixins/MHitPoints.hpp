#ifndef __MIXIN_HIT_POINTS__
#define __MIXIN_HIT_POINTS__

template < typename T >
class MHitPoints {
public:
    MHitPoints(const MHitPoints& hp) : _max(hp._max), _current(hp._current) { }
    MHitPoints(T max, T current = 0) : _max(max), _current(current) { }

    int GetAsPercent() const { return (_current/_max)*100.0; }

    T GetCurrentHP() { return _current; }
    T GetMaxHP() { return _max; }

    void SetMaxHP(T current) { _current = current; }
    void SetCurrentHP(T max) { _max = max; }

    void ReduceHP(T reduce) { _current -=  reduce; }
    void AddHP(T add) {
        _current += add;
        if(_current > _max) { _current = _max; }
    }

    bool isDead() { return _current <= 0; }
    void Heal() { _current = _max; }
private:
    T _max;
    T _current;
};

#endif // __MIXIN_HIT_POINTS__
