#ifndef __TCODMAP_WRAPPER__
#define __TCODMAP_WRAPPER__

#include "libtcod.hpp"

class TCODMapWrapper : public TCODMap
{
public:
    TCODMapWrapper(std::size_t xSize, std::size_t ySize) : TCODMap(xSize, ySize) { }

private:

};

#endif // __TCODMAP_WRAPPER__
