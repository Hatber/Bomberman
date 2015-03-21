#ifndef __BOMBERMAN__
#define __BOMBERMAN__

#include "Engine/GameEngine.hpp"
#include "Gui/GuiEngine.hpp"

#include "libtcod.hpp"

enum GameState {
    GAME,
    STATISTIC,
    TIP,
    EXIT
};

class Bomberman {
public:
    Bomberman();

    void WaitUserInput();
private:
    GameState _currentState;

    GameEngine _engine;
    GuiEngine  _gui;

    void HandleGameAction(TCOD_key_t key);
    void HandleStatisticAction(TCOD_key_t key);
    void HandleTipAction(TCOD_key_t key);
    void HandleExitAction(TCOD_key_t key);
};

#endif // __BOMBERMAN__
