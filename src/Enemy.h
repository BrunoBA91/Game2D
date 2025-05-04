#pragma once
#include "Entity.h"
#include "AnimationController.h"

class AnimationManager; // forward declaration

class Enemy : public Entity {
public:
    Enemy(AnimationManager& animMgr);
    ~Enemy();

    void update(const std::vector<SDL_Rect>& walls) override;
    void render(SDL_Renderer* renderer) override;

private:
    AnimationController animationController;
    float speed = 2.0f;
    int direction = -1; // -1 = left, 1 = right
};