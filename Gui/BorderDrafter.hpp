#ifndef __BORDER_DRAFTER_RLL__
#define __BORDER_DRAFTER_RLL__

#include "libtcod.hpp"

namespace rll {

const TCODColor defaultActiveBorderColor(200,160,30);

class BorderDrafter {
public:
    BorderDrafter() :
        _autoFlush(true), _activeBorderColor(defaultActiveBorderColor) { }
    BorderDrafter(bool autoFlush) :
        _autoFlush(autoFlush), _activeBorderColor(defaultActiveBorderColor) { }

    void SetActiveBorderColor(TCODColor color) { _activeBorderColor = color; }

    void DrawActiveBorder (int startX, int endX, int startY, int endY, TCODConsole* console);
    void DrawPassiveBorder(int startX, int endX, int startY, int endY, TCODConsole* console);

private:
    void Flush(TCODConsole* console);

    bool _autoFlush;
    TCODColor _activeBorderColor;
};

} //rll

#endif // __BORDER_DRAFTER_RLL__
