#include "Game.h"
#include <iostream>

Game::Game()
    : window(nullptr), renderer(nullptr), running(false) {}

Game::~Game() {
    clean();
}

bool Game::init(const std::string& title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return false;
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

    player = new Player();
    if (!player->init(renderer, "assets/player_idle.png", 100, 100, 48, 48)) return false;

    enemy = new Enemy();
    if (!enemy->init(renderer, "assets/player.png", 400, 300, 48, 48)) return false;

    entityManager.add(player);
    entityManager.add(enemy);

    running = true;
    return true;
}

void Game::run() {
    SDL_Event e;
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16); // ~60 FPS
    }
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT ||
           (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            running = false;
        }
    }

    player->handleInput();
}

void Game::update() {
    entityManager.updateAll();
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    entityManager.renderAll(renderer);

    SDL_RenderPresent(renderer);
}

void Game::clean() {
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