#include "Game.h"

int main() {
    Game game;
    
    AssetStyle style = AssetStyle::PixelArt;
    int screenWidth = 1920;
    int screenHeight = 1080;

    if (!game.init("Game2D", style, screenWidth, screenHeight)) return -1;

    game.run();

    return 0;
}
