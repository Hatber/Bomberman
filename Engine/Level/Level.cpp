#include "Level.hpp"

void Level::Clear() {
    for(int i = 0; i < GetYSize(); i++) {
        for(int j = 0; j < GetXSize(); j++) {
            GetType(j, i) = FREE;
        }
    }
}

bool Level::IsWalkable(int x, int y) {
    cellT type = GetType(x, y);
    if( type == FREE || type == BONUS || type == EXIT ) {
        return true;
    }

    return false;
}

bool Level::Transparent(int x, int y) {
    cellT type = GetType(x, y);
    if( type == WALL || type == HERO || type == MONSTER ) {
        return false;
    }

    return true;
}
