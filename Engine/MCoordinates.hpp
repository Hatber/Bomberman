#ifndef __MIXIN_COORDINATES__
#define __MIXIN_COORDINATES__

class MCoordinates : public std::pair< int, int > {
public:
    MCoordinates(int first, int second) : std::pair< int, int >(first, second) { }

    MCoordinates operator - (const MCoordinates& coord) {
        return make_pair(first - coord.first, second - coord.second);
    }

    MCoordinates operator + (const MCoordinates& coord) {
        return make_pair(first + coord.first, second + coord.second);
    }
};

#endif // __MIXIN_COORDINATES__
