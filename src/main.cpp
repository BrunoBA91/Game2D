#include "Game.h"

int main() {
    Game game;
    if (!game.init("Game2D", 800, 600)) return -1;
    game.run();
    return 0;
}
