#ifndef __BOMBERMAN__
#define __BOMBERMAN__

#include "Engine/GameEngine.hpp"
#include "Gui/GuiEngine.hpp"
#include "Util/TCODMapWrapper.hpp"

#include "libtcod.hpp"

enum GameState {
    GAME,
    STATISTIC,
    TIP,
    EXIT_STATE
};

class Bomberman {
public:
    Bomberman();

    void Start();
    void WaitUserInput();
    void Step();

private:
    GameState _currentState;

    GameEngine _engine;
    GuiEngine  _gui;
    TCODMapWrapper _map;

    void FillMapInfo();

    void HandleGameAction(TCOD_key_t key);
    void HandleStatisticAction(TCOD_key_t key);
    void HandleTipAction(TCOD_key_t key);
    void HandleExitAction(TCOD_key_t key);
};

#endif // __BOMBERMAN__
