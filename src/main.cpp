#include "Game.h"

int main() {
    Game game;
    
    AssetStyle style = AssetStyle::PixelArt;
    int screenWidth = 1280;
    int screenHeight = 720;

    if (!game.init("Game2D", style, screenWidth, screenHeight)) return -1;

    game.run();

    return 0;
}
