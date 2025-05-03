#include "Enemy.h"

Enemy::Enemy() : speed(3), direction(1) {}

Enemy::~Enemy() {}

void Enemy::update() {
    rect.x += speed * direction;

    // Reverse direction when hitting screen bounds
    if (rect.x <= 0 || rect.x + rect.w >= 800) { // assuming 800px window width
        direction *= -1;
    }
}