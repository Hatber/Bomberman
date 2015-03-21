#ifndef __GUI_ENGINE__
#define __GUI_ENGINE__

#include "libtcod.hpp"

#include <string>

class GuiEngine {
public:
    GuiEngine();
    void Init(int levelXSize = 50, int levelYSize = 25);

private:
    TCODConsole* _gameWindow;
    TCODConsole* _statWindow;
    TCODConsole* _logWindow;
    TCODConsole* _exitWindow;
    TCODConsole* _tipWindow;

    void DrawInfo();
    void DrawGame();
    void DrawLog();
    void DrawStatPanel();
    void DrawTip(const std::string& text);
    void DrawExit();

    void DrawActiveBorder (int startX, int endX, int startY, int endY);
    void DrawPassiveBorder(int startX, int endX, int startY, int endY);
};

#endif // __GUI_ENGINE__
