#pragma once
#include "Vector2f.h"

struct TransformComponent {
    Vector2f position;
    Vector2f scale = {1.0f, 1.0f};
    float rotation = 0.0f;

    TransformComponent() = default;

    TransformComponent(const Vector2f& pos)
        : position(pos), scale({1.0f, 1.0f}), rotation(0.0f) {}

    TransformComponent(const Vector2f& pos, const Vector2f& scl, float rot)
        : position(pos), scale(scl), rotation(rot) {}
};
