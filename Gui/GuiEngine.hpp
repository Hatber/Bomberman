#ifndef __GUI_ENGINE__
#define __GUI_ENGINE__

#include "../Engine/Level/Level.hpp"
#include "../Util/TCODMapWrapper.hpp"
#include "../Engine/Mixins/MHitPoints.hpp"

#include "../Engine/Bomb.hpp"
#include "BorderDrafter.hpp"

#include "libtcod.hpp"

#include <string>
#include <set>

class GuiEngine {
public:
    GuiEngine();
    void Init(int levelXSize = 50, int levelYSize = 25);

    void DrawInfo(const MHitPoints< float >& currentHP, int score);
    void DrawGame(Level& level, TCODMapWrapper& fovMap);
    void DrawStatPanel(Bomb& bombBuild);

    void DrawExplosion(std::set< MCoordinates >& cover);

    void DrawEndWindow();

private:
    TCODConsole* _gameWindow;
    TCODConsole* _statWindow;
    TCODConsole* _endWindow;

    TCODConsole* _logWindow;
    TCODConsole* _exitWindow;
    TCODConsole* _tipWindow;

    rll::BorderDrafter _border;

    void DrawLog();
    void DrawTip(const std::string& text); //?
    void DrawExit(); //NOPE
};

#endif // __GUI_ENGINE__
