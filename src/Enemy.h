#pragma once
#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy();
    ~Enemy();

    void update() override;

private:
    int speed;
    int direction; // -1 = left, 1 = right
};