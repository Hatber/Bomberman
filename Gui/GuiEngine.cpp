#include "GuiEngine.hpp"
#include "../Common.hpp"

#include "../Util/Format.hpp"

#include <iostream>
using std::cout;
using std::endl;

const TCODColor darkGroundColor(50,50,150);
const TCODColor darkWallColor(0,0,100);

const TCODColor groundColor(130,110,50);
const TCODColor wallColor(200,180,50);

const int HPPanelHeight = 1;
const int BombConfigurationPanelHeight = 7;

GuiEngine::GuiEngine() {
    TCODConsole::initRoot(50 + 2, 25+BombConfigurationPanelHeight+5, "Bomberman", false);
    //TCODConsole::initRoot(50 + 2, 25+3, "Bomberman", false);
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
    _border.DrawActiveBorder(0, levelXSize + 2, HPPanelHeight, levelYSize+3, TCODConsole::root);
    _border.DrawPassiveBorder(0, levelXSize + 2, levelYSize+3, levelYSize + BombConfigurationPanelHeight + HPPanelHeight+4, TCODConsole::root);
}

void GuiEngine::DrawInfo(const MHitPoints< float >& currentHP, int score) {
    std::string panel = "HP : [%c%c%c%c";
    int hpPanelSize = 30;

    int hpPercent = currentHP.GetAsPercent()*hpPanelSize/100;

    if(hpPercent < 0) { return; }

    for(int i = 0; i < hpPercent; i++) { panel += "+"; }
    for(int i = hpPercent; i < hpPanelSize; i++) { panel += " "; }
    panel += "%c]";

    std::string percentStr = " " +  std::string(Format() << currentHP.GetAsPercent()) + "%% ";
    panel.replace(6 + 8 + hpPanelSize/2 - 3, percentStr.size(), percentStr);

    panel += " Score : " + std::string(Format() << score);

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

            if(level.IsClean() && level.GetType(x, y) == EXIT) {
                foreGroundColor = TCODColor::yellow;
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
    _border.DrawPassiveBorder(9, levelXSize - 9, 9, levelYSize - 9, TCODConsole::root);

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

void GuiEngine::DrawStatPanel(Bomb& bombBuild) {
    _border.DrawPassiveBorder(levelXSize - 13, levelXSize+1, -1, 3, _statWindow);
    _statWindow->print(levelXSize - 12, 0, "Press 'h' to\n       help");

    std::string TimerStr = "Timer :";
    for(int i = 1; i<10; i++) {
        if(bombBuild.GetTimer()!=i) {
            TimerStr += " " + std::string(Format() << i) + " ";
        } else {
            TimerStr += " %c%c%c " + std::string(Format() << i) + "%c ";
        }
    }

    std::string PowerStr = "Power :";
    for(int i = 1; i<10; i++) {
        if(bombBuild.GetPower()!=i) {
            PowerStr += " " + std::string(Format() << i) + " ";
        } else {
            PowerStr += " %c%c%c " + std::string(Format() << i) + "%c ";
        }
    }

    std::string DirectStr = "Direct :";
    if(bombBuild.ContainDirection(UP))     { DirectStr += " [UP] "; } else { DirectStr += " [  ] "; }
    if(bombBuild.ContainDirection(DOWN))   { DirectStr += " [DOWN] "; } else { DirectStr += " [    ] "; }
    if(bombBuild.ContainDirection(RIGHT))  { DirectStr += " [RIGHT] "; } else { DirectStr += " [     ] "; }
    if(bombBuild.ContainDirection(LEFT))   { DirectStr += " [LEFT] "; } else { DirectStr += " [    ] "; }
    if(bombBuild.ContainDirection(CENTER)) { DirectStr += " [CENTER] "; } else { DirectStr += " [      ] "; }


    _statWindow->print(1, 1, TimerStr.c_str(),  TCOD_COLCTRL_FORE_RGB, 255, 1, 1, TCOD_COLCTRL_STOP);
    _statWindow->print(1, 3, PowerStr.c_str(),  TCOD_COLCTRL_FORE_RGB, 255, 1, 1, TCOD_COLCTRL_STOP);
    _statWindow->print(1, 5, DirectStr.c_str());
    //_statWindow->print(3, 7, "Build  : A B C D E F G H I J");

    TCODConsole::blit(_statWindow, 0, 0, levelXSize, BombConfigurationPanelHeight, TCODConsole::root, 1, levelYSize + 4);
    TCODConsole::root->flush();
}

void GuiEngine::DrawTip(const std::string& text) { }

void GuiEngine::DrawExit() { }

