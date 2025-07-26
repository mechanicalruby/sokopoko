#ifndef BITMAP_STRING_H
#define BITMAP_STRING_H

// characters per row
#define MONOGRAM_SUBTEX_WIDTH 16

#include "sprite.hpp"
#include "render_list.hpp"
#include <string>
#include <cstring>

namespace Turbine {
void queue_bitmap_string(RenderList& render_list, Texture* texture, const char* string, float x, float y);
}

#endif
