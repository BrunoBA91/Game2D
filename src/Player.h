#pragma once
#include "Entity.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "AnimationController.h"

class Player : public Entity {
public:
    Player(AnimationManager& animMgr);
    ~Player();

    bool init(SDL_Renderer* renderer, const std::string& imagePath, int x, int y, int w, int h) override;
    void handleInput();
    void update(const std::vector<SDL_Rect>& walls, const std::vector<Entity*>& others) override;
    void render(SDL_Renderer* renderer) override;
    void clean() override;

    EntityType getType() const override { return EntityType::Player; }
    
private:
    int speed;
    int vx, vy;
    
    AnimationController animationController;
};