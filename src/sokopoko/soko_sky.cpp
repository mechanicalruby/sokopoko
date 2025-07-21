#include "soko_sky.hpp"

namespace Sokoban {
void Sky::initialize() {
    batch.initialize();

    // create blank white texture data
    std::vector<uint8_t> white_array(64 * 64 * 3, 255);
    Turbine::generate_texture(texture, 64, 64, white_array.data());
    batch.texture = &texture;

    size = {400, 240};
    top_color.color = 0xFFFCCE30;
    bottom_color.color = 0xFFCC5050;
}

void Sky::render() {
    if (batch.texture == nullptr)
        return;

    batch.begin();
    batch.vertices[0] = Vertex{0     , 0     , -1.0f, uv.x / batch.texture->width,               uv.y / batch.texture->height,               top_color.color};
    batch.vertices[1] = Vertex{0     , size.y, -1.0f, uv.x / batch.texture->width,               (uv.y + uv.height) / batch.texture->height, bottom_color.color};
    batch.vertices[2] = Vertex{size.x, size.y, -1.0f, (uv.x + uv.width) / batch.texture->width,  (uv.y + uv.height) / batch.texture->height, bottom_color.color};
    batch.vertices[3] = Vertex{size.x, 0     , -1.0f, (uv.x + uv.width) / batch.texture->width,  uv.y / batch.texture->height,               top_color.color};
    batch.vertices[4] = batch.vertices[2];
    batch.vertices[5] = batch.vertices[0];
    batch.index_offset = 6;
    batch.end();
}
}
