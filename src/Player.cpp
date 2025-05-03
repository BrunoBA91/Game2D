#include "Player.h"

Player::Player() : speed(5), vx(0), vy(0) {}

Player::~Player() {}

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
    rect.x += vx;
    rect.y += vy;
}