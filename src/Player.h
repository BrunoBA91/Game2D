#pragma once
#include "Entity.h"

class Player : public Entity {
public:
    Player();
    ~Player();

    void handleInput();
    void update() override;

private:
    int speed;
    int vx, vy;
};