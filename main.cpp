#include <iostream>
#include <vector>

#include "libtcod.hpp"

using namespace std;

enum cellT {FREE, WALL, BOMB};

struct CellData {
    CellData(
        cellT type      = FREE,
        bool isExplored = false
    ) :
        _type(type),
        _isExplored(isExplored)
    { }

    cellT _type;
    bool _isExplored;
};

class Room {
public:
    Room(size_t xSize, size_t ySize) :
        _room(ySize, vector< CellData >(xSize, CellData()) ) {  }

    cellT& GetType(size_t x, size_t y) { return _room.at(y).at(x)._type; }

    void AddToExplored(size_t x, size_t y) { _room.at(y).at(x)._isExplored = true; }
    bool& IsExplored(size_t x, size_t y)   { return _room.at(y).at(x)._isExplored; }

    size_t getXSize() { return _room.at(0).size(); }
    size_t getYSize() { return _room.size(); }

    bool isWallked(int x, int y) { return GetType(x, y) != WALL; }
    void setBomb(size_t x, size_t y) { GetType(x, y) = BOMB; }

private:
    vector< vector< CellData > > _room;
};

class RoomGenerator {
public:
    RoomGenerator(size_t xSize, size_t ySize) :
        _xSize(xSize), _ySize(ySize) { }

    Room generate() {
        TCODRandom * rnd = TCODRandom::getInstance();
        rnd->setDistribution(TCOD_DISTRIBUTION_LINEAR);
        Room newRoom(_xSize, _ySize);

        size_t wallCount = (newRoom.getXSize()/8)*newRoom.getYSize();
        for(size_t i = 0; i < wallCount; i++) {
            newRoom.GetType(rnd->get(0, newRoom.getXSize()-1), rnd->get(0, newRoom.getYSize()-1)) = WALL;
        }

        for(size_t i = 0; i < 3; i++) {
            for(size_t j = 0; j < 3; j++) {
                newRoom.GetType(i, j) = FREE;
            }
        }

        delete rnd;
        return newRoom;
    }

private:
    size_t _xSize, _ySize;
};

class StandartRoomGenerator {
public:
    StandartRoomGenerator(size_t xSize, size_t ySize) :
        _xSize(xSize), _ySize(ySize) { }

    Room generate() {
        const int step = 7;

        Room newRoom(_xSize, _ySize);

        for(size_t y = step - 1; y < newRoom.getYSize(); y+=step) {
            for(size_t x = step - 1; x < newRoom.getXSize(); x+=step) {
                newRoom.GetType(x, y) = WALL;
            }
        }

        return newRoom;
    }

private:
    size_t _xSize, _ySize;
};

class Map {
public:
    Map(size_t xSize, size_t ySize, const Room& room) :
        _room(room),
        _heroCoord(make_pair(0, 0))
    {
        _tmap = new TCODMap(xSize, ySize);
    }

    ~Map() { delete _tmap; }

    void computeFov() {
        for(size_t i = 0; i < _room.getYSize(); i++) {
            for(size_t j = 0; j < _room.getXSize(); j++) {
                bool isTransparent = true;
                bool isWalkable    = true;

                if(_room.GetType(j, i) == WALL) {
                    isTransparent = false;
                    isWalkable = false;
                }

                _tmap->setProperties(j, i, isTransparent, isWalkable);
            }
        }

        _tmap->computeFov(_heroCoord.first, _heroCoord.second, _fovRadius, true, FOV_DIAMOND);

        for(size_t i = 0; i < _room.getYSize(); i++) {
            for(size_t j = 0; j < _room.getXSize(); j++) {
                if(IsInFov(j, i)) {
                    _room.AddToExplored(j, i);
                }
            }
        }
    }

    bool IsInFov(size_t x, size_t y)       { return _tmap->isInFov(x, y); }
    bool IsTransparent(size_t x, size_t y) { return _tmap->isTransparent(x, y); }
    bool IsWalkable(size_t x, size_t y)    { return _tmap->isWalkable(x, y); }
    bool IsExplored(size_t x, size_t y)    { return _room.IsExplored(x, y); }


    void SetHero(size_t x, size_t y) { _heroCoord = make_pair(x, y); }

private:
    static const int _fovRadius = 5;

    Room _room;
    TCODMap *_tmap;
    pair< size_t, size_t > _heroCoord;
};

const int RightUpCorner = 0XBB;
const int LeftUpCorner = 0XC9;
const int RightDownCorner = 0XBC;
const int LeftDownCorner = 0XC8;
const int HorizontalLine = 0XCD;
const int VerticalLine = 0XBA;

int main() {

    int roomXSize = 50,
        roomYSize = 25;

    StandartRoomGenerator rg(roomXSize, roomYSize);
    Room r(rg.generate());

    Map gameMap(roomXSize, roomYSize, r);

    TCODColor darkGround(50,50,150);
    TCODColor darkWall(0,0,100);

    TCODColor Ground(130,110,50);
    TCODColor Wall(200,180,50);

    TCODColor BorderCollor(200,160,30);

    int playerx=1, playery=1;
    TCODConsole::initRoot(roomXSize + 2, roomYSize + 2 + 10, "Bomberman", false);

    TCODConsole *off1 = new TCODConsole(roomXSize + 2, 10);

     while (!TCODConsole::isWindowClosed()) {
        TCODConsole::root->setDefaultBackground(Wall);
        TCODConsole::root->setDefaultForeground(BorderCollor);

        TCODConsole::root->putChar(0, 0, ' ', TCOD_BKGND_SET);
        TCODConsole::root->putChar(roomXSize + 1, 0, ' ', TCOD_BKGND_SET);

        TCODConsole::root->putChar(0, roomYSize+1, ' ', TCOD_BKGND_SET);
        TCODConsole::root->putChar(roomXSize + 1, roomYSize+1, ' ', TCOD_BKGND_SET);

        for(int i = 0; i < roomXSize; i++) {
            TCODConsole::root->putChar(i+1, 0, ' ', TCOD_BKGND_SET);
            TCODConsole::root->putChar(i+1, roomYSize+1, ' ', TCOD_BKGND_SET);
        }

        for(int i = 0; i < roomYSize; i++) {
            TCODConsole::root->putChar(0, i+1, ' ', TCOD_BKGND_SET);
            TCODConsole::root->putChar(roomXSize+1, i+1, ' ', TCOD_BKGND_SET);
        }


        gameMap.SetHero(playerx - 1, playery - 1);
        gameMap.computeFov();

        for(size_t y = 0; y < r.getYSize(); y++) {

            char roomPresent;
            for(size_t x = 0; x < r.getXSize(); x++) {

                if(gameMap.IsInFov(x, y)) {
                    TCODConsole::root->setDefaultBackground(Ground);
                    TCODConsole::root->setDefaultForeground(Wall);

                    switch(r.GetType(x, y)) {
                        case FREE : roomPresent = ' '; break;
                        case WALL : roomPresent = '#'; break;
                        case BOMB : roomPresent = 'O'; TCODConsole::root->setDefaultForeground(TCODColor::black);  break;
                        default   : roomPresent = (int)13; break;
                    }
                    TCODConsole::root->putChar(x+1, y+1, roomPresent, TCOD_BKGND_SET);

                } else if(gameMap.IsExplored(x, y)) {
                    TCODConsole::root->setDefaultBackground(darkGround);
                    TCODConsole::root->setDefaultForeground(darkWall);
                    switch(r.GetType(x, y)) {
                        case FREE : roomPresent = ' '; break;
                        case WALL : roomPresent = '#'; break;
                        case BOMB : roomPresent = 'O'; TCODConsole::root->setDefaultForeground(TCODColor::white);  break;
                        default   : roomPresent = (int)13; break;
                    }
                    TCODConsole::root->putChar(x+1, y+1, roomPresent, TCOD_BKGND_SET);

                } else {
                    TCODConsole::root->putChar(x+1, y+1, ' ',TCOD_BKGND_BURN);
                }
            }
        }

        TCOD_key_t key;
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL);
        switch(key.vk) {
            case TCODK_UP    : playery!=1         && r.isWallked(playerx-1, playery-2) ? playery-- : 0;break;
            case TCODK_DOWN  : playery!=roomYSize && r.isWallked(playerx-1, playery)   ? playery++ : 0;break;
            case TCODK_LEFT  : playerx!=1         && r.isWallked(playerx-2, playery-1) ? playerx-- : 0;break;
            case TCODK_RIGHT : playerx!=roomXSize && r.isWallked(playerx,   playery-1) ? playerx++ : 0;break;
            case TCODK_SPACE : r.setBomb(playerx-1, playery-1); break;
            default:break;
        }


        TCODConsole::root->setDefaultForeground(TCODColor::green);
        TCODConsole::root->putChar(playerx, playery, '@');


        //Drawing info
        off1->putChar(0, 0, LeftUpCorner);
        off1->putChar(roomXSize+1, 0, RightUpCorner);
        off1->putChar(0, 9, LeftDownCorner);
        off1->putChar(roomXSize+1, 9, RightDownCorner);

        for(int i = 1; i < roomXSize+1; i++) {
            off1->putChar(i, 0, HorizontalLine);
            off1->putChar(i, 9, HorizontalLine);
        }

        for(int i = 1; i < 9; i++) {
            off1->putChar(0, i, VerticalLine);
            off1->putChar(roomXSize+1, i, VerticalLine);
        }

        off1->print(3, 2, "HP     : [%c%c%c%c++++++++++++++++++++%c]", TCOD_COLCTRL_FORE_RGB, 255, 1, 1, TCOD_COLCTRL_STOP);
        off1->print(3, 3, "Timer  : 1 2 3 4 5 6 7 8 9");
        off1->print(3, 4, "Power  : 1 2 3 4 5 6 7 8 9");
        off1->print(3, 5, "Direct : Up Right Down Left Center");
        off1->print(3, 7, "Build  : A B C D E F G H I J");

        TCODConsole::blit(off1,0,0,roomXSize + 2,10, TCODConsole::root, 0, roomYSize + 2);

        TCODConsole::root->flush();

    }

    return 0;
}
