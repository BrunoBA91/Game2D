#include "PhysicsBody.h"
#include <cmath>

PhysicsBody::PhysicsBody() : position(0, 0), velocity(0, 0), rect{0, 0, 0, 0} {}

void PhysicsBody::init(float x, float y, int w, int h) {
    position = {x, y};
    rect = {static_cast<int>(x), static_cast<int>(y), w, h};
}

void PhysicsBody::setPosition(float x, float y) {
    position = {x, y};
    syncRect();
}

void PhysicsBody::setVelocity(float vx, float vy) {
    velocity = {vx, vy};
}

void PhysicsBody::setMaxFallSpeed(float maxSpeed) {
    maxFallSpeed = maxSpeed;
}

Vector2f PhysicsBody::getPosition() const {
    return position;
}

Vector2f PhysicsBody::getVelocity() const {
    return velocity;
}

const SDL_Rect& PhysicsBody::getRect() const {
    return rect;
}

void PhysicsBody::applyGravity(float gravity, float deltaTime) {
    velocity.y += gravity * deltaTime;
    if (velocity.y > maxFallSpeed) {
        velocity.y = maxFallSpeed;
    }
}

void PhysicsBody::update(float deltaTime) {
    // Placeholder if needed later
}

void PhysicsBody::moveWithCollision(const std::vector<SDL_Rect>& walls, Vector2f delta) {
    const float stepSize = 1.0f;
    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    int steps = static_cast<int>(std::ceil(distance / stepSize));
    Vector2f step = (steps > 0) ? Vector2f(delta.x / steps, delta.y / steps) : Vector2f(0.0f, 0.0f);

    for (int i = 0; i < steps; ++i) {
        SDL_Rect next = rect;
        next.x = static_cast<int>(position.x + step.x);
        next.y = static_cast<int>(position.y + step.y);

        bool collided = false;
        for (const SDL_Rect& wall : walls) {
            if (checkAABBCollision(next, wall)) {
                if (std::abs(step.y) >= std::abs(step.x)) {
                    // Y-axis resolution only
                    position.y = (step.y > 0.0f) ? wall.y - rect.h : wall.y + wall.h;
                    velocity.y = 0.0f;
                } else {
                    // X-axis resolution only
                    position.x = (step.x > 0.0f) ? wall.x - rect.w : wall.x + wall.w;
                    velocity.x = 0.0f;
                }
                syncRect();
                return;
            }
        }

        position.x += step.x;
        position.y += step.y;
        syncRect();
    }
}

void PhysicsBody::syncRect() {
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
}

bool PhysicsBody::checkAABBCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return a.x < b.x + b.w &&
           a.x + a.w > b.x &&
           a.y < b.y + b.h &&
           a.y + a.h > b.y;
}