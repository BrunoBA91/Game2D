#include "Game.h"
#include <iostream>
#include <deque>
#include <numeric>
#include "InputManager.h"


Game::Game()
    : window(nullptr), renderer(nullptr), running(false) {}

Game::~Game() {
    clean();
}

bool Game::init(const std::string& title, int width, int height) {
    
    // Adding additional controller types just in case
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
        width, height, SDL_WINDOW_SHOWN);
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

    InputManager::getInstance().initGamepad();

    AnimationManager animMgr;
    animMgr.loadFromFile("assets/animations.json", 64, 64);

    player = new Player(animMgr);
    if (!player->init(renderer, "assets/player_spritesheet.png", 100, 100, 48, 48)) return false;

    enemy = new Enemy(animMgr);
    if (!enemy->init(renderer, "assets/enemy_spritesheet.png", 400, 300, 48, 48)) return false;

    entityManager.add(player);
    entityManager.add(enemy);

    walls = {
        {0, 0, 800, 32},
        {0, 568, 800, 32},
        {100, 450, 50, 50},
        {300, 450, 100, 50},
        {500, 450, 100, 50}
    };

    running = true;
    return true;
}

void Game::run() {
    const int targetFPS = 60;
    const int frameDelay = 1000 / targetFPS;

    std::deque<float> frameTimes;
    const size_t maxSamples = 10;

    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        // Store this frame's delta time
        frameTimes.push_back(deltaTime);
        if (frameTimes.size() > maxSamples) {
            frameTimes.pop_front();
        }

        // Compute average delta time
        deltaTime = std::accumulate(frameTimes.begin(), frameTimes.end(), 0.0f) / frameTimes.size();

        handleEvents();
        update(deltaTime);
        render();

        Uint32 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void Game::handleEvents() {
    InputManager::getInstance().update();
    
    if (InputManager::getInstance().shouldQuit()) {
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

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    InputManager::getInstance().closeGamepad();
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