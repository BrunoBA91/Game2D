#include "Player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Collision.h"

Player::Player(AnimationManager& animMgr) : 
        velocity{0,0}
    {
        animationController.add("idle", animMgr.get("idle"));
        animationController.add("run", animMgr.get("run"));
        animationController.add("jump", animMgr.get("jump"));
        animationController.add("push", animMgr.get("push"));
        animationController.add("pull", animMgr.get("pull"));
        animationController.play("idle");
    }

Player::~Player() {}

bool Player::init(SDL_Renderer* renderer, const std::string& imagePath, int x, int y, int w, int h) {
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load player image: " << IMG_GetError() << "\n";
        return false;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Failed to create player texture: " << SDL_GetError() << "\n";
        return false;
    }

    rect = { x, y, w, h };

    return true;
}

void Player::handleInput() {
    const Uint8* keystate = SDL_GetKeyboardState(nullptr);

    velocity.x = 0;

    if (keystate[SDL_SCANCODE_LEFT]) {
        velocity.x = -moveSpeed;
    } else if (keystate[SDL_SCANCODE_RIGHT]) {
        velocity.x = moveSpeed;
    }

    if (keystate[SDL_SCANCODE_SPACE] && isOnGround) {
        velocity.y = jumpStrength;
        isOnGround = false;
    }
}

void Player::update(const std::vector<SDL_Rect>& walls, const std::vector<Entity*>& others) {
    handleInput();

    velocity.y += gravity;

    applyPhysics(walls);

    if (!isOnGround) {
        animationController.play("jump");
    } else if (velocity.x != 0) {
        animationController.play("run");
    } else {
        animationController.play("idle");
    }
    
    if (velocity.x < 0) {
        flip = SDL_FLIP_HORIZONTAL;
    } else if (velocity.x > 0) {
        flip = SDL_FLIP_NONE;
    }
    animationController.update();
}

void Player::applyPhysics(const std::vector<SDL_Rect>& walls) {
    isOnGround = false;

    resolveHorizontalCollisions(walls);
    resolveVerticalCollisions(walls);
    checkIfStanding(walls);

}

void Player::resolveHorizontalCollisions(const std::vector<SDL_Rect>& walls) {
    SDL_Rect nextX = rect;
    nextX.x += static_cast<int>(velocity.x);

    for (const SDL_Rect& wall : walls) {
        if (Collision::checkAABBCollision(nextX, wall)) {
            if (velocity.x > 0) {
                rect.x = wall.x - rect.w;
            } else if (velocity.x < 0) {
                rect.x = wall.x + wall.w;
            }
            velocity.x = 0;
            return;
        }
    }

    rect.x = nextX.x;
}

void Player::resolveVerticalCollisions(const std::vector<SDL_Rect>& walls) {
    SDL_Rect nextY = rect;
    nextY.y += static_cast<int>(velocity.y);

    for (const SDL_Rect& wall : walls) {
        if (Collision::checkAABBCollision(nextY, wall)) {
            if (velocity.y > 0) {
                rect.y = wall.y - rect.h;
            } else if (velocity.y < 0) {
                rect.y = wall.y + wall.h;
            }
            velocity.y = 0;
            return;
        }
    }

    rect.y = nextY.y;
}

void Player::checkIfStanding(const std::vector<SDL_Rect>& walls) {
    SDL_Rect probe = rect;
    probe.y += 1;

    for (const SDL_Rect& wall : walls) {
        if (Collision::checkAABBCollision(probe, wall)) {
            isOnGround = true;
            return;
        }
    }

    isOnGround = false;
}



void Player::render(SDL_Renderer* renderer) {
    SDL_Rect srcRect = animationController.getCurrentFrameRect();

    SDL_RenderCopyEx(renderer, texture, &srcRect, &rect, 0.0, nullptr, getFlip());
}

void Player::clean() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}