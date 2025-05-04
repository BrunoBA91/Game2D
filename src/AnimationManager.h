#pragma once
#include <string>
#include <unordered_map>
#include "Animation.h"
#include "json.hpp"

class AnimationManager {
public:
    bool loadFromFile(const std::string& filePath, int defaultFrameWidth, int defaultFrameHeight);
    const Animation& get(const std::string& name) const;
    bool has(const std::string& name) const;

private:
    std::unordered_map<std::string, Animation> animations;
};