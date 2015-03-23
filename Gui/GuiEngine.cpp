#include "GuiEngine.hpp"
#include "../Common.hpp"

const int rightUpCorner   = 0XBB;
const int leftUpCorner    = 0XC9;
const int rightDownCorner = 0XBC;
const int leftDownCorner  = 0XC8;
const int horizontalLine  = 0XCD;
const int verticalLine    = 0XBA;

const TCODColor activeBorderColor(200,160,30);

const TCODColor darkGroundColor(50,50,150);
const TCODColor darkWallColor(0,0,100);

const TCODColor groundColor(130,110,50);
const TCODColor wallColor(200,180,50);

const int HPPanelHeight = 1;
const int BombConfigurationPanelHeight = 10;

GuiEngine::GuiEngine() {
    TCODConsole::initRoot(50 + 2, 25+11+4, "Bomberman", false);
}

void GuiEngine::Init(int levelXSize, int levelYSize) {
    _gameWindow = new TCODConsole(levelXSize, levelYSize);
    _statWindow = new TCODConsole(levelXSize, BombConfigurationPanelHeight);
    _logWindow  = new TCODConsole(levelXSize, BombConfigurationPanelHeight);
    _exitWindow = new TCODConsole(levelXSize / 3, levelYSize / 3);
    _tipWindow  = new TCODConsole(levelXSize / 2, levelYSize / 2);

    DrawLog();
    DrawInfo();
    DrawActiveBorder(0, levelXSize + 2, HPPanelHeight, levelYSize+3);
    DrawPassiveBorder(0, levelXSize + 2, levelYSize+3, levelYSize + BombConfigurationPanelHeight + HPPanelHeight+4);
}

void GuiEngine::DrawInfo() {
    TCODConsole::root->print(0, 0, " HP : [%c%c%c%c++++++++++++++++++++%c]    Score: 100500", TCOD_COLCTRL_FORE_RGB, 255, 1, 1, TCOD_COLCTRL_STOP);
    //TCODConsole::root->print(10, 10, "%c%c%c%cFUCK YOU, UNIVERSE!%c", TCOD_COLCTRL_FORE_RGB, 255, 1, 1, TCOD_COLCTRL_STOP);
    TCODConsole::root->flush();
}

void GuiEngine::DrawGame(Level& level, TCODMapWrapper& fovMap) {
    for(int y = 0; y < level.GetYSize(); y++) {
        char roomPresent;

        for(int x = 0; x < level.GetXSize(); x++) {
            switch(level.GetType(x, y)) {
                case FREE    : roomPresent = ' '; break;
                case WALL    : roomPresent = '#'; break;
                case HERO    : roomPresent = '@'; break;
                case BOMB    : roomPresent = 'O'; break;
                case BONUS   : roomPresent = '$'; break;
                case EXIT    : roomPresent = '>'; break;
                case MONSTER : roomPresent = 'M'; break;
            }

            if(fovMap.isInFov(x, y)) {
                _gameWindow->setDefaultBackground(groundColor);
                _gameWindow->setDefaultForeground(wallColor);
            } else if(level.IsExplored(x, y)) {
                _gameWindow->setDefaultBackground(darkGroundColor);
                _gameWindow->setDefaultForeground(darkWallColor);
            } else {
                _gameWindow->setDefaultForeground(TCODColor::black);
                _gameWindow->setDefaultBackground(TCODColor::black);
            }

            _gameWindow->putChar(x,y,roomPresent, TCOD_BKGND_SET);
        }
    }

    TCODConsole::blit(_gameWindow, 0, 0, levelXSize, levelYSize, TCODConsole::root, 1, 2);
    TCODConsole::root->flush();
}

void GuiEngine::DrawLog() {
    for(int i = 0; i < 10; i++) {
        _logWindow->print(0, i, "%c%c%c This is test phrase to log panel!%c", TCOD_COLCTRL_FORE_RGB, 1, 255/(i+1), 1, TCOD_COLCTRL_STOP);
    }

    TCODConsole::blit(_logWindow, 0, 0, levelXSize, BombConfigurationPanelHeight, TCODConsole::root, 1, levelYSize + 4);
    TCODConsole::root->flush();
}

void GuiEngine::DrawStatPanel() {

}

void GuiEngine::DrawTip(const std::string& text) {

}

void GuiEngine::DrawExit() {

}

void GuiEngine::DrawActiveBorder (int startX, int endX, int startY, int endY) {
    TCODConsole::root->setDefaultBackground(activeBorderColor);

    for(int i = startX; i < endX; i++) {
        TCODConsole::root->putChar(i, startY, ' ', TCOD_BKGND_SET);
        TCODConsole::root->putChar(i, endY - 1,   ' ', TCOD_BKGND_SET);
    }

    for(int i = startY; i < endY; i++) {
        TCODConsole::root->putChar(startX, i, ' ', TCOD_BKGND_SET);
        TCODConsole::root->putChar(endX - 1,   i, ' ', TCOD_BKGND_SET);
    }

    TCODConsole::root->flush();
}

void GuiEngine::DrawPassiveBorder(int startX, int endX, int startY, int endY) {
    TCODConsole::root->setDefaultBackground(TCODColor::black);

    TCODConsole::root->putChar(startX,   startY,   leftUpCorner);
    TCODConsole::root->putChar(endX - 1, startY,   rightUpCorner);
    TCODConsole::root->putChar(startX,   endY - 1, leftDownCorner);
    TCODConsole::root->putChar(endX - 1, endY - 1, rightDownCorner);

    for(int i = startX + 1; i < endX - 1; i++) {
        TCODConsole::root->putChar(i, startY,   horizontalLine, TCOD_BKGND_SET);
        TCODConsole::root->putChar(i, endY - 1, horizontalLine, TCOD_BKGND_SET);
    }

    for(int i = startY + 1; i < endY - 1; i++) {
        TCODConsole::root->putChar(startX,   i, verticalLine, TCOD_BKGND_SET);
        TCODConsole::root->putChar(endX - 1, i, verticalLine, TCOD_BKGND_SET);
    }

    TCODConsole::root->flush();
}

