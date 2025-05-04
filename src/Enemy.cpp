#include "Enemy.h"
#include "AnimationManager.h"

Enemy::Enemy(AnimationManager& animMgr) {
    animationController.add("idle", animMgr.get("enemy_idle"));
    animationController.add("run", animMgr.get("enemy_run"));
    animationController.play("run");

    rect = { 300, 100, 64, 64 }; // starting position
    setFlip(SDL_FLIP_HORIZONTAL); // face left initially
}

Enemy::~Enemy() {}

void Enemy::update(const std::vector<SDL_Rect>& walls, const std::vector<Entity*>& others) {
    
    (void)walls;
    (void)others;
    
    rect.x += speed * direction;

    if (rect.x <= 0 || rect.x + rect.w >= 800) {
        direction *= -1;
        setFlip(direction < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    }

    animationController.play("run");
    animationController.update();
}

void Enemy::render(SDL_Renderer* renderer) {
    SDL_Rect srcRect = animationController.getCurrentFrameRect();
    SDL_RenderCopyEx(renderer, texture, &srcRect, &rect, 0.0, nullptr, getFlip());
}