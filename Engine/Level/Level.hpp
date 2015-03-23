#ifndef __LEVEL__
#define __LEVEL__

#include "../Mixins/MCoordinates.hpp"

#include <vector>

enum cellT {FREE, BONUS, EXIT, BOMB, WALL, HERO, MONSTER};

class Level  {
public:
    Level(int xSize, int ySize) :
        _room(ySize, std::vector< CellData >(xSize, CellData()) ) {  }

    void Clear();

    cellT& GetType(int x, int y) { return _room.at(y).at(x)._type; }

    void AddToExplored(int x, int y) { _room.at(y).at(x)._isExplored = true; }
    bool& IsExplored(int x, int y)   { return _room.at(y).at(x)._isExplored; }

    int GetXSize() { return _room.at(0).size(); }
    int GetYSize() { return _room.size(); }

    bool IsWalkable(int x, int y);
    bool Transparent(int x, int y);


    void SetBonus(const MCoordinates& coord)   { SetType(coord, BONUS); }
    void SetExit(const MCoordinates& coord)    { SetType(coord, EXIT); }
    void SetBomb(const MCoordinates& coord)    { SetType(coord, BOMB); }
    void SetWall(const MCoordinates& coord)    { SetType(coord, WALL); }
    void SetHero(const MCoordinates& coord)    { SetType(coord, HERO); }
    void SetMonster(const MCoordinates& coord) { SetType(coord, MONSTER); }

private:
    void SetType(const MCoordinates& coord, cellT type) { GetType(coord.first, coord.second) = type; }

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

    std::vector< std::vector< CellData > > _room;
};

#endif // __LEVEL__
