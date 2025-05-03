#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "Player.h"
#include "Enemy.h"
#include "EntityManager.h"

class Game {
public:
    Game();
    ~Game();

    bool init(const std::string& title, int width, int height);
    void run();
    void clean();

private:
    void handleEvents();
    void update();
    void render();

    SDL_Window* window;
    SDL_Renderer* renderer;

    EntityManager entityManager;
    Player* player;
    Enemy* enemy;

    bool running;
};