#pragma once
#include <SDL2/SDL.h>

namespace Collision {
    // Axis-Aligned Bounding Box collision check
    inline bool checkAABBCollision(const SDL_Rect& a, const SDL_Rect& b) {
        return a.x + a.w > b.x &&
               b.x + b.w > a.x &&
               a.y + a.h > b.y &&
               b.y + b.h > a.y;
    }
}
