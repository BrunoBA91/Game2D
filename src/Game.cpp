#include <SDL2/SDL_image.h>
#include <iostream>
#include <deque>
#include <numeric>
#include "Game.h"
#include "Player.h"
#include "Enemy.h"

const int VIRTUAL_WIDTH = 960;
const int VIRTUAL_HEIGHT = 540;

Game::Game()
    : window(nullptr), renderer(nullptr), running(false) {}

Game::~Game() {
    clean();
}

bool Game::init(const std::string& title, AssetStyle style, int windowWidth, int windowHeight) {
    assetStyle = style;

    preferredWindowWidth = windowWidth;
    preferredWindowHeight = windowHeight;

    // Set scaling quality based on asset style
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, (assetStyle == AssetStyle::PixelArt) ? "0" : "2");

    // Adding additional game controller types just in case
    SDL_GameControllerAddMappingsFromFile("assets/gamecontrollerdb.txt");
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return false;
    }
    
    if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) != 0) {
        SDL_Log("SDL_INIT_JOYSTICK failed: %s", SDL_GetError());
    }

    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0) {
        SDL_Log("SDL_INIT_GAMECONTROLLER failed: %s", SDL_GetError());
    }
    
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Set logical size
    SDL_RenderSetLogicalSize(renderer, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    // Enable integer scaling for pixel art
    if (assetStyle == AssetStyle::PixelArt) {
        SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
    } 

    inputManager.initGamepad();

    animationManager.loadFromFile("assets/animations.json", 64, 64);

    player = new Player(resourceManager, inputManager, animationManager);
    if (!player->init(renderer, "assets/player_spritesheet.png", 100, 100, 48, 48)) return false;

    enemy  = new Enemy(resourceManager, animationManager);
    if (!enemy->init(renderer, "assets/enemy_spritesheet.png", 400, 300, 48, 48)) return false;

    entityManager.add(player);
    entityManager.add(enemy);

    walls = {
        {0+10, 0+10, VIRTUAL_WIDTH - 20, 32},
        {0+10, VIRTUAL_HEIGHT - 32 - 10, VIRTUAL_WIDTH - 20, 32},
        {100, 400, 50, 50},
        {300, 400, 100, 50},
        {500, 400, 100, 50}
    };

    running = true;
    return true;
}

void Game::run() {

    timeManager.setTargetFPS(60);
    timeManager.setMaxDeltaTime(0.05f);
    timeManager.setSmoothingWindow(10);

    while (running) {
        timeManager.update();
        float deltaTime = timeManager.getSmoothedDeltaTime();

        handleEvents();
        update(deltaTime);
        render();
        
    }
}

void Game::handleEvents() {
    inputManager.update(window, preferredWindowWidth, preferredWindowHeight);
    if (inputManager.shouldQuit()) {
        running = false;
    }
}

void Game::update(float deltaTime) {
    entityManager.updateAll(walls, deltaTime);
}

void Game::render() {  
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (const SDL_Rect& wall : walls) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawRect(renderer, &wall);
    }  

    entityManager.renderAll(renderer);

    // Draw a green debug border around the virtual screen
    SDL_Rect debugBorder = {0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // bright green
    SDL_RenderDrawRect(renderer, &debugBorder);

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    inputManager.closeGamepad();
    entityManager.cleanAll();

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    IMG_Quit();
    SDL_Quit();
}