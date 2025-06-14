#pragma once
#include "Entity.h"
#include "PhysicsBody.h"
#include "AnimationController.h"
#include "TransformComponent.h"

// Forward declarations
class AnimationManager;
class ResourceManager; 

class Enemy : public Entity {
public:
    Enemy(ResourceManager& resMgr, AnimationManager& animMgr);
    ~Enemy();

    bool init(SDL_Renderer* renderer, const std::string& imagePath, int x, int y, int w, int h) override;
    void update(const std::vector<SDL_Rect>& walls,
                const std::vector<Entity*>& others,
                float deltaTime) override;
    void render(SDL_Renderer* renderer) override;
    void clean() override;

    TransformComponent transform;

    EntityType getType() const override { return EntityType::Enemy; }

    void setFlip(SDL_RendererFlip f) override { flip = f; }
    SDL_RendererFlip getFlip() const override { return flip; }

private:
    SDL_Texture* texture = nullptr;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    PhysicsBody physicsBody;
    AnimationController animationController;

    ResourceManager& resourceManager;
    AnimationManager& animationManager;

    float speed = 40.0f;
    int direction = -1;  // -1 = left, 1 = right
};
