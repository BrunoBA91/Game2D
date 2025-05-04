#include "Animation.h"

Animation::Animation()
    : frameWidth(0), frameHeight(0), totalFrames(1),
      currentFrame(0), frameDuration(100), elapsedTime(0), rowIndex(0) {}

Animation::Animation(int frameW, int frameH, int numFrames, int frameTime, int row)
    : frameWidth(frameW), frameHeight(frameH), totalFrames(numFrames),
      currentFrame(0), frameDuration(frameTime), elapsedTime(0), rowIndex(row) {}

void Animation::update() {
    elapsedTime += 16; // assuming ~60 FPS
    if (elapsedTime >= frameDuration) {
        elapsedTime = 0;
        currentFrame = (currentFrame + 1) % totalFrames;
    }
}

void Animation::reset() {
    currentFrame = 0;
    elapsedTime = 0;
}

void Animation::setFrame(int frame) {
    if (frame >= 0 && frame < totalFrames) {
        currentFrame = frame;
        elapsedTime = 0;
    }
}

void Animation::setRow(int row) {
    rowIndex = row;
}

SDL_Rect Animation::getCurrentFrameRect() const {
    SDL_Rect srcRect = {
        currentFrame * frameWidth,
        rowIndex * frameHeight,
        frameWidth,
        frameHeight
    };
    return srcRect;
}