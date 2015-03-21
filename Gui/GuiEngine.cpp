#include "GuiEngine.hpp"

const int rightUpCorner   = 0XBB;
const int leftUpCorner    = 0XC9;
const int rightDownCorner = 0XBC;
const int leftDownCorner  = 0XC8;
const int horizontalLine  = 0XCD;
const int verticalLine    = 0XBA;

const TCODColor activeBorderColor(200,160,30);

const int HPPanelHeight = 1;
const int BombConfigurationPanelHeight = 10;

const int levelXSize = 50;
const int levelYSize = 25;

GuiEngine::GuiEngine() {
    TCODConsole::initRoot(50, 25+11, "Bomberman", false);
}

void GuiEngine::Init(int levelXSize, int levelYSize) {
    _gameWindow = new TCODConsole(levelXSize, levelYSize);
    _statWindow = new TCODConsole(levelXSize, BombConfigurationPanelHeight);
    _logWindow  = new TCODConsole(levelXSize, BombConfigurationPanelHeight);
    _exitWindow = new TCODConsole(levelXSize / 3, levelYSize / 3);
    _tipWindow  = new TCODConsole(levelXSize / 2, levelYSize / 2);

    DrawLog();
    DrawInfo();
    DrawActiveBorder(0, levelXSize, HPPanelHeight, levelYSize);
    DrawPassiveBorder(0, levelXSize, levelYSize, levelYSize + BombConfigurationPanelHeight + HPPanelHeight);
}

void GuiEngine::DrawInfo() {
    TCODConsole::root->print(0, 0, " HP : [%c%c%c%c++++++++++++++++++++%c]    Score: 100500", TCOD_COLCTRL_FORE_RGB, 255, 1, 1, TCOD_COLCTRL_STOP);
    //TCODConsole::root->print(10, 10, "%c%c%c%cFUCK YOU, UNIVERSE!%c", TCOD_COLCTRL_FORE_RGB, 255, 1, 1, TCOD_COLCTRL_STOP);
    TCODConsole::root->flush();
}

void GuiEngine::DrawGame() {

}

void GuiEngine::DrawLog() {
    _logWindow->print(0, 0, "%c%c%c This is test phrase to log panel!%c", TCOD_COLCTRL_FORE_RGB, 1, 255/1, 1, TCOD_COLCTRL_STOP);
    _logWindow->print(0, 1, "%c%c%c This is test phrase to log panel!%c", TCOD_COLCTRL_FORE_RGB, 1, 255/2, 1, TCOD_COLCTRL_STOP);
    _logWindow->print(0, 2, "%c%c%c This is test phrase to log panel!%c", TCOD_COLCTRL_FORE_RGB, 1, 255/3, 1, TCOD_COLCTRL_STOP);
    _logWindow->print(0, 3, "%c%c%c This is test phrase to log panel!%c", TCOD_COLCTRL_FORE_RGB, 1, 255/4, 1, TCOD_COLCTRL_STOP);
    _logWindow->print(0, 4, "%c%c%c This is test phrase to log panel!%c", TCOD_COLCTRL_FORE_RGB, 1, 255/5, 1, TCOD_COLCTRL_STOP);
    _logWindow->print(0, 5, "%c%c%c This is test phrase to log panel!%c", TCOD_COLCTRL_FORE_RGB, 1, 255/6, 1, TCOD_COLCTRL_STOP);
    _logWindow->print(0, 6, "%c%c%c This is test phrase to log panel!%c", TCOD_COLCTRL_FORE_RGB, 1, 255/7, 1, TCOD_COLCTRL_STOP);
    _logWindow->print(0, 7, "%c%c%c This is test phrase to log panel!%c", TCOD_COLCTRL_FORE_RGB, 1, 255/8, 1, TCOD_COLCTRL_STOP);
    _logWindow->print(0, 8, "%c%c%c This is test phrase to log panel!%c", TCOD_COLCTRL_FORE_RGB, 1, 255/9, 1, TCOD_COLCTRL_STOP);
    _logWindow->print(0, 9, "%c%c%c This is test phrase to log panel!%c", TCOD_COLCTRL_FORE_RGB, 1, 255/10, 1, TCOD_COLCTRL_STOP);


    TCODConsole::blit(_logWindow, 0, 0, levelXSize, BombConfigurationPanelHeight, TCODConsole::root, 1, levelYSize + 1);
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

