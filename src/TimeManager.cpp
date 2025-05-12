#include "TimeManager.h"
#include <algorithm>

TimeManager::TimeManager()
    : deltaTime(0.0f), maxDeltaTime(0.05f), 
      lastFrameTime(SDL_GetTicks()), frameWindowSize(10),
      frameStartTime(0), targetFPS(std::nullopt) {}

void TimeManager::update() {
    frameStartTime = SDL_GetTicks();
    
    Uint32 currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastFrameTime) / 1000.0f; // ms to seconds
    lastFrameTime = currentTime;

    deltaTime = std::min(deltaTime, maxDeltaTime); // cap delta

    frameTimes.push_back(deltaTime);
    if (frameTimes.size() > frameWindowSize)
        frameTimes.pop_front();

    // FPS limiter
    enforceFrameCap();
}

float TimeManager::getDeltaTime() const {
    return deltaTime;
}

float TimeManager::getSmoothedDeltaTime() const {
    if (frameTimes.empty()) return deltaTime;
    float sum = std::accumulate(frameTimes.begin(), frameTimes.end(), 0.0f);
    return sum / frameTimes.size();
}

void TimeManager::setMaxDeltaTime(float max) {
    maxDeltaTime = max;
}

void TimeManager::setSmoothingWindow(size_t frames) {
    frameWindowSize = std::max<size_t>(1, frames);
}

int TimeManager::getFPS() const {
    float smoothed = getSmoothedDeltaTime();
    return smoothed > 0.0f ? static_cast<int>(1.0f / smoothed) : 0;
}

void TimeManager::enforceFrameCap() {
    if (!targetFPS.has_value()) return;

    Uint32 frameDelay = 1000 / targetFPS.value();
    Uint32 frameTime = SDL_GetTicks() - frameStartTime;
    if (frameTime < frameDelay) {
        SDL_Delay(frameDelay - frameTime);
    }
}

void TimeManager::disableFrameCap() {
    targetFPS.reset();
}

void TimeManager::setTargetFPS(int fps) {
    targetFPS = std::max(1, fps); // avoid division by zero
}
