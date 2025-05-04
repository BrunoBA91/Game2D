#include "Player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

Player::Player(AnimationManager& animMgr) : 
    speed(5), vx(0), vy(0)
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

    if (keys[SDL_SCANCODE_LEFT]) {
        setFlip(SDL_FLIP_HORIZONTAL);
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        setFlip(SDL_FLIP_NONE);
    }
        

    if (moving) {
        animationController.play("run");
    } else {
        animationController.play("idle");
    }

    animationController.update();

    rect.x += vx;
    rect.y += vy;
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