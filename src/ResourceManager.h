#pragma once
#include <string>
#include <unordered_map>


// Forward declarations
struct SDL_Texture;
struct SDL_Renderer;

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    void loadTexture(const std::string& id, const std::string& path, SDL_Renderer* renderer);
    SDL_Texture* getTexture(const std::string& id) const;
    void clear();

private:
    std::unordered_map<std::string, SDL_Texture*> textures;
};
