#pragma once
#include <SDL2/SDL.h>

class Animation {
public:
    Animation();
    Animation(int frameW, int frameH, int numFrames, int frameTime, int row = 0, int spacingX = 0, int spacingY = 0);

    void update();
    void reset();
    void setFrame(int frame);  // optional
    void setRow(int row);      // switch animation row

    SDL_Rect getCurrentFrameRect() const;

private:
    int frameWidth;
    int frameHeight;
    int totalFrames;
    int currentFrame;
    int frameDuration;   // ms per frame
    int elapsedTime;     // ms since last frame change
    int rowIndex;        // vertical row in sprite sheet
    int spacingX;
    int spacingY;
};