#ifndef RENDER_LIST_HPP
#define RENDER_LIST_HPP

#include "sprite.hpp"

#include <iostream>
#include <unordered_map>

namespace Turbine {
struct RenderList {
public:
    RenderList() = default;
    void queue(Sprite& sprite);
    void begin(void);
    void end(void);
private:
    std::unordered_map<uint32_t, Batch> batches;
    Texture* last_texture = nullptr;
};
}
#endif
