#include "EntityManager.h"

void EntityManager::add(Entity* entity) {
    entities.push_back(entity);
}

void EntityManager::updateAll(const std::vector<SDL_Rect>& walls) {
    for (auto& e : entities) {
        e->update(walls, entities);
    }
}

const std::vector<Entity*>& EntityManager::getEntities() const {
    return entities;
}

void EntityManager::renderAll(SDL_Renderer* renderer) {
    for (auto& e : entities) {
        e->render(renderer);
    }
}

void EntityManager::cleanAll() {
    for (auto& e : entities) {
        if (e) {
            e->clean();
            delete e; // assumes dynamic allocation
        }
    }
    entities.clear();
}

EntityManager::~EntityManager() {
    cleanAll(); // fail-safe on destruction
}