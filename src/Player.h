#pragma once
#include "Entity.h"
#include "Animation.h"

class Player : public Entity {
public:
    Player();
    ~Player();

    bool init(SDL_Renderer* renderer, const std::string& imagePath, int x, int y, int w, int h) override;
    void handleInput();
    void update() override;
    void render(SDL_Renderer* renderer) override;
    void clean() override;

private:
    int speed;
    int vx, vy;
    
    Animation idleAnimation;
};