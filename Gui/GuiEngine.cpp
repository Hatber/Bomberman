#include "GuiEngine.hpp"
#include "../Common.hpp"

#include "../Util/Format.hpp"

#include <iostream>
using std::cout;
using std::endl;

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
    //TCODConsole::initRoot(50 + 2, 25+11+4, "Bomberman", false);
    TCODConsole::initRoot(50 + 2, 25+3, "Bomberman", false);
}

void GuiEngine::Init(int levelXSize, int levelYSize) {
    _gameWindow = new TCODConsole(levelXSize, levelYSize);
    _statWindow = new TCODConsole(levelXSize, BombConfigurationPanelHeight);
    _endWindow  = new TCODConsole(30, 5);

    _logWindow  = new TCODConsole(levelXSize, BombConfigurationPanelHeight);
    _exitWindow = new TCODConsole(levelXSize / 3, levelYSize / 3);
    _tipWindow  = new TCODConsole(levelXSize / 2, levelYSize / 2);

    //DrawLog();
    //DrawInfo(MHitPoints<float>(defaultMaxHP), 0);
    DrawActiveBorder(0, levelXSize + 2, HPPanelHeight, levelYSize+3);
    DrawPassiveBorder(0, levelXSize + 2, levelYSize+3, levelYSize + BombConfigurationPanelHeight + HPPanelHeight+4);
}

void GuiEngine::DrawInfo(const MHitPoints< float >& currentHP, int score) {
    std::string panel = "HP : [%c%c%c%c";
    int hpPanelSize = 34;

    int hpPercent = currentHP.GetAsPercent()*hpPanelSize/100;

    if(hpPercent < 0) { return; }

    for(int i = 0; i < hpPercent; i++) { panel += "+"; }
    for(int i = hpPercent; i < hpPanelSize; i++) { panel += " "; }
    panel += "%c]";

    panel += " Level : " + std::string(Format() << score);


    TCODConsole::root->print(0, 0, panel.c_str(), TCOD_COLCTRL_FORE_RGB, 255, 1, 1, TCOD_COLCTRL_STOP);
    TCODConsole::root->flush();
}

void GuiEngine::DrawGame(Level& level, TCODMapWrapper& fovMap) {
    for(int y = 0; y < level.GetYSize(); y++) {
        char roomPresent = ' ';

        for(int x = 0; x < level.GetXSize(); x++) {
            switch(level.GetType(x, y)) {
                case FREE    : roomPresent = ' '; break;
                case WALL    : roomPresent = '#'; break;
                case HERO    : roomPresent = '@'; break;
                case BOMB    : roomPresent = 'O'; break;
                case BONUS   : roomPresent = '$'; break;
                case EXIT    : roomPresent = '>'; break;

                case MONSTER_N : roomPresent = 'N'; break;
                case MONSTER_F : roomPresent = 'F'; break;
                case MONSTER_S : roomPresent = 'S'; break;
            }

            TCODColor backGroundColor;
            TCODColor foreGroundColor;

            if(fovMap.isInFov(x, y)) {
                backGroundColor = groundColor;
                foreGroundColor = wallColor;
            } else if(level.IsExplored(x, y)) {
                backGroundColor = darkGroundColor;
                foreGroundColor = darkWallColor;
            } else {
                backGroundColor = TCODColor::black;
                foreGroundColor = TCODColor::black;
            }

            if(level.GetType(x, y) == MONSTER_N) {
                foreGroundColor = TCODColor::green;
            }

            if(level.GetType(x, y) == MONSTER_F) {
                foreGroundColor = TCODColor::red;
            }

            if(level.GetType(x, y) == MONSTER_S) {
                foreGroundColor = TCODColor::blue;
            }

            _gameWindow->setDefaultBackground(backGroundColor);
            _gameWindow->setDefaultForeground(foreGroundColor);

            _gameWindow->putChar(x,y,roomPresent, TCOD_BKGND_SET);
        }
    }

    TCODConsole::blit(_gameWindow, 0, 0, levelXSize, levelYSize, TCODConsole::root, 1, 2);
    TCODConsole::root->flush();
}

void GuiEngine::DrawExplosion(std::set< MCoordinates >& cover) {
    _gameWindow->setDefaultBackground(TCODColor::lightYellow);
    _gameWindow->setDefaultForeground(TCODColor::red);

    std::set< MCoordinates >::iterator it;
    for(it = cover.begin(); it!=cover.end(); it++) {
        _gameWindow->putChar(it->first, it->second, _gameWindow->getChar(it->first, it->second), TCOD_BKGND_SET);
    }

    TCODConsole::blit(_gameWindow, 0, 0, levelXSize, levelYSize, TCODConsole::root, 1, 2);
    TCODConsole::root->flush();
}

void GuiEngine::DrawEndWindow() {
    DrawPassiveBorder(9, levelXSize - 9, 9, levelYSize - 9);

    _endWindow->setDefaultForeground(TCODColor::red);
    _endWindow->setDefaultBackground(TCODColor::black);

    for(int y = 0; y < 5; y++) {
        for(int x = 0; x < 30; x++) {
            _endWindow->putChar(x, y, ' ', TCOD_BKGND_SET);
        }
    }

    _endWindow->print(0, 1, "Congratulations! You are dead!");
    _endWindow->print(0, 3, "   Press 'Esc' to exit...");

    TCODConsole::blit(_endWindow, 0, 0, 30, 5, TCODConsole::root, 10, 10);
    TCODConsole::root->flush();
}

void GuiEngine::DrawLog() {
    for(int i = 0; i < 10; i++) {
        _logWindow->print(0, i, "%c%c%c This is test phrase to log panel!%c", TCOD_COLCTRL_FORE_RGB, 1, 255/(i+1), 1, TCOD_COLCTRL_STOP);
    }

    TCODConsole::blit(_logWindow, 0, 0, levelXSize, BombConfigurationPanelHeight, TCODConsole::root, 1, levelYSize + 4);
    TCODConsole::root->flush();
}

void GuiEngine::DrawStatPanel(GameEngine& engine) {
    std::string TimerStr = "Timer  : ";
    for(int i = 1; i<10; i++) {
        if(engine._findedTimers.find(i) != engine._findedTimers.end()) {
            //if(i == engine._currentBombBuild)
        }
    }

    _statWindow->print(3, 2, "Timer  : 1 2 3 4 5 6 7 8 9");
    _statWindow->print(3, 3, "Power  : 1 2 3 4 5 6 7 8 9");
    _statWindow->print(3, 5, "Direct : [Up] [Right] [Down] [Left] [Center]");
    _statWindow->print(3, 7, "Build  : A B C D E F G H I J");

    TCODConsole::blit(_statWindow, 0, 0, levelXSize, BombConfigurationPanelHeight, TCODConsole::root, 1, levelYSize + 4);
    TCODConsole::root->flush();
}

void GuiEngine::DrawTip(const std::string& text) { }

void GuiEngine::DrawExit() { }

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

