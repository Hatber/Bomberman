#include "Bomberman.hpp"
#include "Common.hpp"

Bomberman::Bomberman() :
    _currentState(GAME),
    _map(levelXSize, levelYSize)
{
    _gui.Init();

    _engine.UpdateLevel();
    UpdateTCODMap();
    _gui.DrawGame(_engine.GetLevel(), _map);
}

void Bomberman::Start() {
    while (!TCODConsole::isWindowClosed()) {
        WaitUserInput();

        _engine.UpdateLevel();
        UpdateTCODMap();
        _gui.DrawGame(_engine.GetLevel(), _map);
    }
}

void Bomberman::WaitUserInput() {
    TCOD_key_t key;
    key = TCODConsole::waitForKeypress(true);

    switch(_currentState) {
        case GAME       : HandleGameAction(key); break;
        case STATISTIC  : break;
        case TIP        : HandleTipAction(key); break;
        case EXIT_STATE : HandleExitAction(key); break;
    }
}

void Bomberman::Step() {

}

void Bomberman::UpdateTCODMap() {
    Level& currentLevel = _engine.GetLevel();
    for(int y = 0; y < currentLevel.GetYSize(); y++) {
        for(int x = 0; x < currentLevel.GetXSize(); x++) {
            _map.setProperties(x, y, currentLevel.IsTransparent(x, y), currentLevel.IsWalkable(x, y));
        }
    }

    MCoordinates heroPosition = _engine.GetHeroPosition();
    _map.computeFov(heroPosition.first, heroPosition.second, 5);

    for(int y = 0; y < currentLevel.GetYSize(); y++) {
        for(int x = 0; x < currentLevel.GetXSize(); x++) {
            if(_map.isInFov(x, y)) {
                currentLevel.AddToExplored(x, y);
            }
        }
    }
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
