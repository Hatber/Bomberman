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
        _engine.UpdateLevel();

        UpdateTCODMap();

        _gui.DrawInfo(_engine.GetHeroHP(), 0);
        _gui.DrawGame(_engine.GetLevel(), _map);
        //_gui.DrawStatPanel();

        if(!WaitUserInput()) { continue; }

        _engine.Step(_map);

        _gui.DrawGame(_engine.GetLevel(), _map);
        _gui.DrawExplosion(_engine.explosionCover);
        if(!_engine.explosionCover.empty()) { TCODSystem::sleepMilli(100); }

        if(_engine.GetHeroHP().isDead()) {
            _gui.DrawEndWindow();
            TCOD_key_t key;
            do {
                key = TCODConsole::waitForKeypress(false);
            } while (key.vk != TCODK_ESCAPE);
            return;
        }

        _engine.PutToDeath();
    }
}

bool Bomberman::WaitUserInput() {
    TCOD_key_t key;
    key = TCODConsole::checkForKeypress();
    //TCODConsole::waitForKeypress(false);

    switch(_currentState) {
        case GAME       : return HandleGameAction(key); break;
        case STATISTIC  : break;
        case TIP        : HandleTipAction(key); break;
        case EXIT_STATE : HandleExitAction(key); break;
    }

    return false;
}

void Bomberman::Step() {
    _engine.Step(_map);
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

bool Bomberman::HandleGameAction(TCOD_key_t key) {
    bool isComlete = false;

    if(key.vk == TCODK_UP)          { isComlete = _engine.MoveHero(0, -1); }
    else if(key.vk == TCODK_RIGHT)  { isComlete = _engine.MoveHero(1, 0); }
    else if(key.vk == TCODK_DOWN)   { isComlete = _engine.MoveHero(0, 1); }
    else if(key.vk == TCODK_LEFT)   { isComlete = _engine.MoveHero(-1, 0); }
    else if(key.c == 'w')           { isComlete = _engine.MoveHero(0, 0); }
    else if(key.vk == TCODK_SPACE)  { isComlete = _engine.PlaceBomb(); }
    //else if(key.c == 'a')           { isComlete = _engine.DoAction(); }
    //else if(key.vk == TCODK_ESCAPE) {}
    //else if(key.vk == TCODK_TAB)    {}

    return isComlete;
}

bool Bomberman::HandleStatisticAction(TCOD_key_t key) {
    bool isNeedTime = false;

    return isNeedTime;
}

bool Bomberman::HandleTipAction(TCOD_key_t key) { return true; }
bool Bomberman::HandleExitAction(TCOD_key_t key) { return true; }
