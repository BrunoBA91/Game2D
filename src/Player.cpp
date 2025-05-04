#include "Player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

Player::Player(AnimationManager& animMgr) : 
    speed(5), vx(0), vy(0)
    {
        idleAnimation = animMgr.get("idle");
        runAnimation = animMgr.get("run");
        currentAnimation = &idleAnimation;
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
    const Uint8* keystates = SDL_GetKeyboardState(nullptr);

    vx = 0;
    vy = 0;

    if (keystates[SDL_SCANCODE_UP])    vy = -speed;
    if (keystates[SDL_SCANCODE_DOWN])  vy =  speed;
    if (keystates[SDL_SCANCODE_LEFT])  vx = -speed;
    if (keystates[SDL_SCANCODE_RIGHT]) vx =  speed;
}

void Player::update() {
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    bool moving =
        keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_DOWN] ||
        keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_RIGHT];

    if (!moving) {
        currentAnimation = &idleAnimation;
    } else {
        currentAnimation = &runAnimation;
    }

    currentAnimation->update();

    rect.x += vx;
    rect.y += vy;
}

void Player::render(SDL_Renderer* renderer) {
    SDL_Rect srcRect = currentAnimation->getCurrentFrameRect();

    SDL_RenderCopy(renderer, texture, &srcRect, &rect);
}

void Player::clean() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}