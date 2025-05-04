#include "AnimationManager.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool AnimationManager::loadFromFile(const std::string& filePath, int defaultFrameWidth, int defaultFrameHeight) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open animation config file: " << filePath << std::endl;
        return false;
    }

    json data;
    try {
        file >> data;
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse animation config: " << e.what() << std::endl;
        return false;
    }

    for (const auto& anim : data["animations"]) {
        std::string name = anim["name"];
        int row = anim["row"];
        int frames = anim["frames"];
        int frameTime = anim["frameTime"];
        int frameW = anim.value("width", defaultFrameWidth);
        int frameH = anim.value("height", defaultFrameHeight);
        int spacingX = anim.value("spacingX", 0);
        int spacingY = anim.value("spacingY", 0);

        animations[name] = Animation(frameW, frameH, frames, frameTime, row, spacingX, spacingY);
    }

    return true;
}

const Animation& AnimationManager::get(const std::string& name) const {
    return animations.at(name);
}

bool AnimationManager::has(const std::string& name) const {
    return animations.find(name) != animations.end();
}