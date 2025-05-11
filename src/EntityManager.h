#pragma once
#include <vector>

// Forward declarations
class Entity;

class EntityManager {
public:
    ~EntityManager();

    void add(Entity* entity);
    void updateAll(const std::vector<SDL_Rect>& walls, float deltaTime);
    void renderAll(SDL_Renderer* renderer);
    void cleanAll();

    const std::vector<Entity*>& getEntities() const;

private:
    std::vector<Entity*> entities;
};