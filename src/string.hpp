#ifndef BITMAP_STRING_H
#define BITMAP_STRING_H

// characters per row
#define MONOGRAM_SUBTEX_WIDTH 16

#include "sprite.hpp"
#include <string>

namespace Turbine {
void queue_bitmap_string(Batch& batch, const char* string, float x, float y);
}

#endif
