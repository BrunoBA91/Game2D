#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "EntityManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "AnimationManager.h"
#include "TimeManager.h"

enum class AssetStyle { PixelArt, HDArt };

//Forward declarations
class Player;
class Enemy;

class Game {
public:
    Game();
    ~Game();

    bool init(const std::string& title, AssetStyle style, int windowWidth, int windowHeight);
    void run();
    void clean();

private:
    void handleEvents();
    void update(float deltaTime);
    void render();

    SDL_Window* window;
    SDL_Renderer* renderer;

    ResourceManager resourceManager;
    InputManager inputManager;
    EntityManager entityManager;
    AnimationManager animationManager;
    TimeManager timeManager;
    
    Player* player;
    Enemy* enemy;

    bool running;

    const float maxDeltaTime = 0.05f;

    AssetStyle assetStyle;

    int preferredWindowWidth;
    int preferredWindowHeight;

    std::vector<SDL_Rect> walls;
};