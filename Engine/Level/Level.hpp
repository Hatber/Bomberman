#ifndef __LEVEL__
#define __LEVEL__

#include "../Mixins/MCoordinates.hpp"

#include <vector>

enum cellT {FREE, BONUS, EXIT, BOMB, WALL, HERO, MONSTER_N, MONSTER_F, MONSTER_S};

class Level  {
public:
    Level(int xSize, int ySize) :
        _room(ySize, std::vector< CellData >(xSize, CellData()) ) {  }

    void Clear();
    void FullClear();

    cellT& GetType(int x, int y) { return _room.at(y).at(x)._type; }

    void AddToExplored(int x, int y) { _room.at(y).at(x)._isExplored = true; }
    bool IsExplored(int x, int y)   { return _room.at(y).at(x)._isExplored; }
    void UnExplored(int x, int y)   { _room.at(y).at(x)._isExplored = false; }

    int GetXSize() { return _room.at(0).size(); }
    int GetYSize() { return _room.size(); }

    bool IsWalkable(int x, int y);
    bool IsTransparent(int x, int y);


    void SetBonus(const MCoordinates& coord) { SetType(coord, BONUS); }
    void SetExit(const MCoordinates& coord)  { SetType(coord, EXIT); }
    void SetBomb(const MCoordinates& coord)  { SetType(coord, BOMB); }
    void SetWall(const MCoordinates& coord)  { SetType(coord, WALL); }
    void SetHero(const MCoordinates& coord)  { SetType(coord, HERO); }

    void SetMonsterNormal(const MCoordinates& coord) { SetType(coord, MONSTER_N); }
    void SetMonsterSlow(const MCoordinates& coord)   { SetType(coord, MONSTER_S); }
    void SetMonsterFast(const MCoordinates& coord)   { SetType(coord, MONSTER_F); }

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
