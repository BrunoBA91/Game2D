#include "Player.h"
#include "Collision.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

Player::Player(AnimationManager& animMgr) {
    animationController.add("idle", animMgr.get("idle"));
    animationController.add("run", animMgr.get("run"));
    animationController.add("jump", animMgr.get("jump"));
    animationController.play("idle");
}

Player::~Player() {
    clean();
}

bool Player::init(SDL_Renderer* renderer, const std::string& imagePath, int x, int y, int w, int h) {
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        SDL_Log("Failed to load player sprite: %s", IMG_GetError());
        return false;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        SDL_Log("Failed to create player texture: %s", SDL_GetError());
        return false;
    }

    physics.init(static_cast<float>(x), static_cast<float>(y), w, h);
    physics.setMaxFallSpeed(maxFallSpeed);
    return true;
}

void Player::update(const std::vector<SDL_Rect>& walls,
                    const std::vector<Entity*>& others,
                    float deltaTime) {
    handleInput();

    physics.applyGravity(gravity, deltaTime);
    physics.moveWithCollision(walls, physics.getVelocity() * deltaTime);

    checkIfStanding(walls);

    if (isOnGround) {
        coyoteTimer = coyoteTime;  // reset when grounded
    } else {
        coyoteTimer -= deltaTime;  // count down
    }

    const auto& vel = physics.getVelocity();
    if (!isOnGround) {
        animationController.play("jump");
    } else if (vel.x != 0.0f) {
        animationController.play("run");
        setFlip(vel.x > 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
    } else {
        animationController.play("idle");
    }

    animationController.update();
}

void Player::render(SDL_Renderer* renderer) {
    SDL_Rect srcRect = animationController.getCurrentFrameRect();
    SDL_RenderCopyEx(renderer, texture, &srcRect, &getRect(), 0.0, &origin, flip);
}

void Player::clean() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Player::handleInput() {
    const Uint8* keystate = SDL_GetKeyboardState(nullptr);

    Vector2f velocity = physics.getVelocity();
    velocity.x = 0;

    if (keystate[SDL_SCANCODE_LEFT]) {
        velocity.x = -moveSpeed;
    } else if (keystate[SDL_SCANCODE_RIGHT]) {
        velocity.x = moveSpeed;
    }

    if (keystate[SDL_SCANCODE_SPACE] && coyoteTimer > 0.0f) {
        velocity.y = jumpStrength;
        isOnGround = false;
        coyoteTimer = 0.0f; // Consume coyote jump
    }

    physics.setVelocity(velocity.x, velocity.y);
}

void Player::checkIfStanding(const std::vector<SDL_Rect>& walls) {
    SDL_Rect probe = physics.getRect();
    probe.y += 1;
    isOnGround = false;

    for (const SDL_Rect& wall : walls) {
        if (Collision::checkAABBCollision(probe, wall)) {
            isOnGround = true;
            break;
        }
    }
}
