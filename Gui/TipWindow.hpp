#ifndef __TIP_WINDOW_RLL__
#define __TIP_WINDOW_RLL__

#include "libtcod.hpp"
#include <string>
#include <vector>

namespace rll {

class TipWindow : public TCODConsole {
public:
    TipWindow(int xSize, int ySize) : TCODConsole(xSize, ySize), _currentTipNum(0) { }

    bool LoadFromFile(const std::string& fileName);
    bool Add(const std::string& tip);

    void Prev();
    void Next();

private:
    class Tip {
    public:

    private:
        std::string _name;
        std::string _content;
    };

    void DrawTip(int tipNum);

    int _currentTipNum;
    std::vector< Tip > _tips;
};

} //rll

#endif // __TIP_WINDOW_RLL__
