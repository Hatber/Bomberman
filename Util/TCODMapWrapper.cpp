#include "TCODMapWrapper.hpp"

void TCODMapWrapper::SetTransparent(int x, int y, bool isTransparent) {
    setProperties(x, y, isTransparent, isWalkable(x, y));
}

void TCODMapWrapper::TCODMapWrapper::SetWalkable(int x, int y, bool isWalkable) {
    setProperties(x, y, isTransparent(x, y), isWalkable);
}
