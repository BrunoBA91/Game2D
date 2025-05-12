#pragma once
#include <deque>
#include <numeric>
#include <optional>
#include <SDL2/SDL.h>

class TimeManager {
public:
    TimeManager();

    void update();

    float getDeltaTime() const;
    float getSmoothedDeltaTime() const;
    int getFPS() const;
    void disableFrameCap();
    void setMaxDeltaTime(float max);
    void setSmoothingWindow(size_t frames);
    void setTargetFPS(int fps);

private:
    void enforceFrameCap();

private:
    float deltaTime;
    float maxDeltaTime;
    Uint32 lastFrameTime;

    std::optional<int> targetFPS;
    Uint32 frameStartTime;

    std::deque<float> frameTimes;
    size_t frameWindowSize;
};