#include "Bomberman.hpp"
#include "Common.hpp"

Bomberman::Bomberman() :
    _currentState(GAME),
    _map(levelXSize, levelYSize)
{
    _gui.Init();
}

void Bomberman::Start() {
    while (!TCODConsole::isWindowClosed()) {
        WaitUserInput();
    }
}

void Bomberman::WaitUserInput() {
    TCOD_key_t key;
    key = TCODConsole::waitForKeypress(true);

    switch(_currentState) {
        case GAME      : HandleGameAction(key); break;
        case STATISTIC : break;
        case TIP       : HandleTipAction(key); break;
        case EXIT      : HandleExitAction(key); break;
    }
}

void Bomberman::Step() {

}

void Bomberman::HandleGameAction(TCOD_key_t key) {
    bool isComlete = false;

    if(key.vk == TCODK_UP)          { isComlete = _engine.MoveHero(0, -1); }
    else if(key.vk == TCODK_RIGHT)  { isComlete = _engine.MoveHero(1, 0); }
    else if(key.vk == TCODK_DOWN)   { isComlete = _engine.MoveHero(0, 1); }
    else if(key.vk == TCODK_LEFT)   { isComlete = _engine.MoveHero(-1, 0); }
    else if(key.c == 'w')           { isComlete = _engine.MoveHero(0, 0); }
    else if(key.vk == TCODK_SPACE)  { isComlete = _engine.PlaceBomb(); }
    else if(key.c == 'a')           { isComlete = _engine.DoAction(); }
    else if(key.vk == TCODK_ESCAPE) {}
    else if(key.vk == TCODK_TAB)    {}
}

void Bomberman::HandleStatisticAction(TCOD_key_t key) {
    bool isNeedTime = false;
}

void Bomberman::HandleTipAction(TCOD_key_t key) { }
void Bomberman::HandleExitAction(TCOD_key_t key) { }
