#ifndef SOKOBAN_SKY_HPP
#define SOKOBAN_SKY_HPP

#include "../sprite.hpp"
#include "../texture.hpp"

using namespace Turbine;

namespace Sokoban {
struct Sky {
    Batch batch;
    Texture* texture;
    Vector2 size;
    Rect uv;
    Color top_color;
    Color bottom_color;
    // add system for stars

    Sky() = default;
    void initialize(Texture* texture);
    void render();
};
}

#endif
