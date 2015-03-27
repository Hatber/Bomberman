#include "BorderDrafter.hpp"

using namespace rll;

namespace rll {

const int rightUpCorner   = 0XBB;
const int leftUpCorner    = 0XC9;
const int rightDownCorner = 0XBC;
const int leftDownCorner  = 0XC8;
const int horizontalLine  = 0XCD;
const int verticalLine    = 0XBA;

} //rll

void BorderDrafter::DrawActiveBorder (int startX, int endX, int startY, int endY, TCODConsole* console) {
    console->setDefaultBackground(_activeBorderColor);

    for(int i = startX; i < endX; i++) {
        console->putChar(i, startY, ' ', TCOD_BKGND_SET);
        console->putChar(i, endY - 1,   ' ', TCOD_BKGND_SET);
    }

    for(int i = startY; i < endY; i++) {
        console->putChar(startX, i, ' ', TCOD_BKGND_SET);
        console->putChar(endX - 1,   i, ' ', TCOD_BKGND_SET);
    }

    Flush(console);
}

void BorderDrafter::DrawPassiveBorder(int startX, int endX, int startY, int endY, TCODConsole* console) {
    console->setDefaultBackground(TCODColor::black);

    console->putChar(startX,   startY,   leftUpCorner);
    console->putChar(endX - 1, startY,   rightUpCorner);
    console->putChar(startX,   endY - 1, leftDownCorner);
    console->putChar(endX - 1, endY - 1, rightDownCorner);

    for(int i = startX + 1; i < endX - 1; i++) {
        console->putChar(i, startY,   horizontalLine, TCOD_BKGND_SET);
        console->putChar(i, endY - 1, horizontalLine, TCOD_BKGND_SET);
    }

    for(int i = startY + 1; i < endY - 1; i++) {
        console->putChar(startX,   i, verticalLine, TCOD_BKGND_SET);
        console->putChar(endX - 1, i, verticalLine, TCOD_BKGND_SET);
    }

    Flush(console);
}

void BorderDrafter::Flush(TCODConsole* console) {
    if(_autoFlush) {
        console->flush();
    }
}
