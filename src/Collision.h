#pragma once
#include <SDL2/SDL.h>

namespace Collision {
    bool AABB(const SDL_Rect& a, const SDL_Rect& b);
}