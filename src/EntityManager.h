#pragma once
#include <vector>
#include "Entity.h"

class EntityManager {
public:
    ~EntityManager();

    void add(Entity* entity); // Entity* can be Player*, Enemy*, etc.
    void updateAll();
    void renderAll(SDL_Renderer* renderer);
    void cleanAll();

private:
    std::vector<Entity*> entities;
};