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
    static const int _fovRadius = 20;

    Room _room;
    TCODMap *_tmap;
    pair< size_t, size_t > _heroCoord;
};


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
    TCODConsole::initRoot(roomXSize + 2, roomYSize + 2, "Bomberman", false);

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
        TCODConsole::root->putChar(playerx, playery,'@');

        TCODConsole::root->flush();
    }

    return 0;
}
