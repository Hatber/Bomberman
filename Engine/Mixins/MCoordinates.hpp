#ifndef __MIXIN_COORDINATES__
#define __MIXIN_COORDINATES__

#include <utility>

class MCoordinates : public std::pair< int, int > {
public:
    MCoordinates() : std::pair< int, int >(0, 0) { }
    MCoordinates(int first, int second) : std::pair< int, int >(first, second) { }
    MCoordinates(std::pair< int, int > base) : std::pair< int, int >(base) { }

    MCoordinates operator - (const MCoordinates& coord) {
        return std::make_pair(first - coord.first, second - coord.second);
    }

    MCoordinates operator + (const MCoordinates& coord) {
        return std::make_pair(first + coord.first, second + coord.second);
    }
};

#endif // __MIXIN_COORDINATES__
