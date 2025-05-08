#include "Enemy.h"
#include "AnimationManager.h"
#include <SDL2/SDL_image.h>

Enemy::Enemy(AnimationManager& animMgr) {
    animationController.add("idle", animMgr.get("enemy_idle"));
    animationController.add("run", animMgr.get("enemy_run"));
    animationController.play("run");

    physicsBody.attachTransform(&transform);
}

Enemy::~Enemy() {
    clean();
}

bool Enemy::init(SDL_Renderer* renderer, const std::string& imagePath, int x, int y, int w, int h) {
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        SDL_Log("Failed to load enemy sprite: %s", IMG_GetError());
        return false;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        SDL_Log("Failed to create enemy texture: %s", SDL_GetError());
        return false;
    }

    physicsBody.init(static_cast<float>(x), static_cast<float>(y), w, h);
    return true;
}

void Enemy::update(const std::vector<SDL_Rect>& walls,
                   const std::vector<Entity*>& others,
                   float deltaTime) {
    (void)others;

    Vector2f velocity = physicsBody.getVelocity();
    velocity.x = speed * direction;
    physicsBody.setVelocity(velocity.x, velocity.y);

    physicsBody.moveWithCollision(walls, velocity * deltaTime);

    const SDL_Rect& rect = physicsBody.getRect();
    if (rect.x <= 0 || rect.x + rect.w >= 800) {
        direction *= -1;
        setFlip(direction < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    }

    animationController.play("run");
    animationController.update();
}

void Enemy::render(SDL_Renderer* renderer) {
    SDL_Rect srcRect = animationController.getCurrentFrameRect();

    SDL_Rect dstRect;
    dstRect.x = static_cast<int>(transform.position.x);
    dstRect.y = static_cast<int>(transform.position.y);
    dstRect.w = srcRect.w;
    dstRect.h = srcRect.h;

    SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, 0.0, &origin, flip);
}

void Enemy::clean() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}
