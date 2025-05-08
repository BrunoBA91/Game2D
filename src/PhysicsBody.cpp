#include "PhysicsBody.h"
#include <cmath>
#include "Collision.h"
#include "TransformComponent.h"

PhysicsBody::PhysicsBody() : velocity(0, 0), rect{0, 0, 0, 0} {}

void PhysicsBody::attachTransform(TransformComponent* t) {
    transform = t;
}

TransformComponent* PhysicsBody::getTransform() const {
    return transform;
}

void PhysicsBody::init(float x, float y, int w, int h) {
    if (!transform) return;
    
    transform->position = {x, y};
    rect = {static_cast<int>(x), static_cast<int>(y), w, h};
}

void PhysicsBody::setPosition(float x, float y) {
    if (!transform) return;
    
    transform->position = {x, y};
    syncRect();
}

void PhysicsBody::setVelocity(float vx, float vy) {
    velocity = {vx, vy};
}

void PhysicsBody::setMaxFallSpeed(float maxSpeed) {
    maxFallSpeed = maxSpeed;
}

Vector2f PhysicsBody::getPosition() const {
    return transform ? transform->position : Vector2f(0, 0);
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
    if (!transform) return;

    transform->position += velocity * deltaTime;
    syncRect();
}

void PhysicsBody::moveWithCollision(const std::vector<SDL_Rect>& walls, Vector2f delta) {
    if (!transform) return;
    
    // Optimization: skip if there's no movement requested
    if (delta.x == 0.0f && delta.y == 0.0f) return;

    // Summary:
    // Incrementally moves the entity along the delta vector,
    // resolving collisions along X and Y separately. This prevents
    // tunneling and gives better control over collision resolution order.

    const float stepSize = 1.0f;
    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    int steps = static_cast<int>(std::ceil(distance / stepSize));
    Vector2f step = (steps > 0) ? Vector2f(delta.x / steps, delta.y / steps) : Vector2f(0.0f, 0.0f);

    for (int i = 0; i < steps; ++i) {
        // Move in X and check collision
        transform->position.x += step.x;
        syncRect();
        for (const SDL_Rect& wall : walls) {
            if (checkAABBCollision(wall)) {
                if (step.x > 0.0f) {
                    transform->position.x = wall.x - rect.w;
                } else if (step.x < 0.0f) {
                    transform->position.x = wall.x + wall.w;
                }
                velocity.x = 0.0f;
                syncRect();
                break;
            }
        }

        // Move in Y and check collision
        transform->position.y += step.y;
        syncRect();
        for (const SDL_Rect& wall : walls) {
            if (checkAABBCollision(wall)) {
                if (step.y > 0.0f) {
                    transform->position.y = wall.y - rect.h;
                } else if (step.y < 0.0f) {
                    transform->position.y = wall.y + wall.h;
                }
                velocity.y = 0.0f;
                syncRect();
                break;
            }
        }
    }
}

void PhysicsBody::syncRect() {
    if (!transform) return;
    
    rect.x = static_cast<int>(transform->position.x);
    rect.y = static_cast<int>(transform->position.y);
}

bool PhysicsBody::checkAABBCollision(const SDL_Rect& other) const {
    return Collision::checkAABBCollision(this->rect, other);
}