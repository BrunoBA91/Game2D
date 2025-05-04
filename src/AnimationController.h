#pragma once
#include <string>
#include <unordered_map>
#include "Animation.h"

class AnimationController {
public:
    void add(const std::string& name, const Animation& animation);
    void play(const std::string& name, bool resetIfSame = false);
    void update();
    void reset();
    SDL_Rect getCurrentFrameRect() const;

    const std::string& getCurrentName() const;
    bool has(const std::string& name) const;

private:
    std::unordered_map<std::string, Animation> animations;
    Animation* currentAnimation = nullptr;
    std::string currentName;
};