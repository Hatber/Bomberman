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
    bool WaitUserInput();
    void Step();

private:
    void UpdateTCODMap();

    GameState _currentState;

    GameEngine _engine;
    GuiEngine  _gui;
    TCODMapWrapper _map;

    void FillMapInfo();

    bool HandleGameAction(TCOD_key_t key);
    bool HandleStatisticAction(TCOD_key_t key);
    bool HandleTipAction(TCOD_key_t key);
    bool HandleExitAction(TCOD_key_t key);
};

#endif // __BOMBERMAN__
