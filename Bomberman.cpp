#include "Bomberman.hpp"
#include "libtcod.hpp"

Bomberman::Bomberman() {}

void Bomberman::WaitUserInput() {
    TCODConsole::waitForKeypress(true);
}

