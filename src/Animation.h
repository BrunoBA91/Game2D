#pragma once
#include <SDL2/SDL.h>

class Animation {
public:
    Animation();
    Animation(int frameW, int frameH, int numFrames, int frameTime);

    void update();
    void reset();
    void setFrame(int frame);  // optional

    SDL_Rect getCurrentFrameRect() const;

private:
    int frameWidth;
    int frameHeight;
    int totalFrames;
    int currentFrame;
    int frameDuration;   // ms per frame
    int elapsedTime;     // ms since last frame change
};