#ifndef __MIXIN_COORDINATES__
#define __MIXIN_COORDINATES__

#include <utility>

class MCoordinates : public std::pair< int, int > {
public:
    MCoordinates() : std::pair< int, int >(0, 0) { }
    MCoordinates(int first, int second) : std::pair< int, int >(first, second) { }
    MCoordinates(std::pair< int, int > base) : std::pair< int, int >(base) { }

    void SetCoordinate(const MCoordinates& coord) { SetCoordinate(coord.first, coord.second); }
    void SetCoordinate(int x, int y) {
        first = x;
        second = y;
    }

    void ReduceCoordinate(int x, int y) { *this = *this - MCoordinates(std::make_pair(x, y)); }
    void AddCoordinate(int x, int y) { *this = *this + MCoordinates(std::make_pair(x, y)); }

    bool CompareCoordinate(const MCoordinates& coord) { return *this == coord; }
    bool InBorders(const MCoordinates& x, const MCoordinates& y) {
        return *this >= x && *this <= y;
    }

    MCoordinates operator - (const MCoordinates& coord) {
        return std::make_pair(first - coord.first, second - coord.second);
    }

    MCoordinates operator + (const MCoordinates& coord) {
        return std::make_pair(first + coord.first, second + coord.second);
    }

private:

};

#endif // __MIXIN_COORDINATES__
