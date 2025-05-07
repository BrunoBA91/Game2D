#pragma once
#include "Entity.h"
#include "PhysicsBody.h"
#include "AnimationController.h"
#include "AnimationManager.h"
#include <SDL2/SDL.h>

class Player : public Entity {
public:
    Player(AnimationManager& animMgr);
    ~Player();

    bool init(SDL_Renderer* renderer, const std::string& imagePath, int x, int y, int w, int h) override;
    void update(const std::vector<SDL_Rect>& walls,
                const std::vector<Entity*>& others,
                float deltaTime) override;

    void render(SDL_Renderer* renderer) override;
    void clean() override;

    EntityType getType() const override { return EntityType::Player; }
    void setFlip(SDL_RendererFlip f) override { flip = f; }
    SDL_RendererFlip getFlip() const override { return flip; }
    void setOrigin(int x, int y) override { origin = {x, y}; }

    const SDL_Rect& getRect() const override { return physics.getRect(); }

private:
    SDL_Texture* texture = nullptr;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Point origin = {0, 0};

    PhysicsBody physics;
    AnimationController animationController;

    float gravity = 1500.0f;
    float maxFallSpeed = 900.0f;
    float moveSpeed = 300.0f;
    float jumpStrength = -650.0f;
    bool isOnGround = false;

    float coyoteTime = 0.1f;
    float coyoteTimer = 0.0f;

    float jumpBufferTime = 0.1f;   // 100 ms window
    float jumpBufferTimer = 0.0f;  // countdown
    bool wantsToJump = false;      // player pressed jump


    void handleInput();
    void checkIfStanding(const std::vector<SDL_Rect>& walls);
};
