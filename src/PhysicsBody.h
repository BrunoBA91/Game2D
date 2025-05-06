#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "Vector2f.h"

class PhysicsBody {
public:
    PhysicsBody();

    void init(float x, float y, int w, int h);

    void update(float deltaTime);
    void moveWithCollision(const std::vector<SDL_Rect>& walls, Vector2f delta);

    const SDL_Rect& getRect() const;
    void setPosition(float x, float y);
    void setVelocity(float vx, float vy);
    void setMaxFallSpeed(float maxSpeed);
    void applyGravity(float gravity, float deltaTime);

    Vector2f getPosition() const;
    Vector2f getVelocity() const;

private:
    Vector2f position;
    Vector2f velocity;
    float maxFallSpeed = 500.0f;
    SDL_Rect rect;

    void syncRect();
    bool checkAABBCollision(const SDL_Rect& a, const SDL_Rect& b);
};