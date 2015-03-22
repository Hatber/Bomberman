#ifndef __TCODMAP_WRAPPER__
#define __TCODMAP_WRAPPER__

#include "libtcod.hpp"

class TCODMapWrapper : public TCODMap
{
public:
    TCODMapWrapper(int xSize, int ySize) : TCODMap(xSize, ySize) { }

    void SetTransparent(int x, int y, bool isTransparent = true);
    void SetWalkable(int x, int y, bool isWalkable = true);
private:

};

#endif // __TCODMAP_WRAPPER__
