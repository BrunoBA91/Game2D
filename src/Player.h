#pragma once
#include "Entity.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "AnimationController.h"

class Player : public Entity {
public:
    Player(AnimationManager& animMgr);
    ~Player();

    bool init(SDL_Renderer* renderer, const std::string& imagePath, int x, int y, int w, int h) override;
    void handleInput();
    void update(const std::vector<SDL_Rect>& walls) override;
    void render(SDL_Renderer* renderer) override;
    void clean() override;

private:
    int speed;
    int vx, vy;
    
    AnimationController animationController;
};