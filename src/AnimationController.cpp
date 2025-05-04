#include "AnimationController.h"
#include <iostream>

void AnimationController::add(const std::string& name, const Animation& animation) {
    animations[name] = animation;
    if (!currentAnimation) {
        currentAnimation = &animations[name];
        currentName = name;
    }
}

void AnimationController::play(const std::string& name, bool resetIfSame) {
    if (name == currentName && !resetIfSame) return;
    auto it = animations.find(name);
    if (it != animations.end()) {
        currentAnimation = &it->second;
        currentName = name;
        currentAnimation->reset();
    } else {
        std::cerr << "Animation '" << name << "' not found in controller.\n";
    }
}

void AnimationController::update() {
    if (currentAnimation) {
        currentAnimation->update();
    }
}

void AnimationController::reset() {
    if (currentAnimation) {
        currentAnimation->reset();
    }
}

SDL_Rect AnimationController::getCurrentFrameRect() const {
    return currentAnimation ? currentAnimation->getCurrentFrameRect() : SDL_Rect{0, 0, 0, 0};
}

const std::string& AnimationController::getCurrentName() const {
    return currentName;
}

bool AnimationController::has(const std::string& name) const {
    return animations.find(name) != animations.end();
}