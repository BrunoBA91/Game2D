#include "Game.h"

int main() {
    Game game;
    AssetStyle style = AssetStyle::PixelArt;
    if (!game.init("Game2D", style, 800, 600)) return -1;
    game.run();
    return 0;
}
