#pragma once
#include "Entity.h"
#include "AnimationController.h"

class AnimationManager; // forward declaration

class Enemy : public Entity {
public:
    Enemy(AnimationManager& animMgr);
    ~Enemy();

    void update(const std::vector<SDL_Rect>& walls, const std::vector<Entity*>& others) override;
    void render(SDL_Renderer* renderer) override;

    EntityType getType() const override { return EntityType::Enemy; }

private:
    AnimationController animationController;
    float speed = 2.0f;
    int direction = -1; // -1 = left, 1 = right
};