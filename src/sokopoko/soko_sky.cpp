#include "soko_sky.hpp"

namespace Sokoban {
void Sky::initialize(Texture* texture) {
    if(texture == nullptr)
        return;
    
    batch.initialize();
    batch.texture = texture;
    this->texture = texture;
    size = {400, 240};
    uv   = {167, 0, 4, 4};
    top_color.color = 0x00000000;
    bottom_color.color = 0x00000000;
}

void Sky::render() {
    batch.begin();
    batch.vertices[0] = Vertex{0     , 0     , -1.0f, uv.x / texture->width,               uv.y / texture->height,               top_color.color};
    batch.vertices[1] = Vertex{0     , size.y, -1.0f, uv.x / texture->width,               (uv.y + uv.height) / texture->height, bottom_color.color};
    batch.vertices[2] = Vertex{size.x, size.y, -1.0f, (uv.x + uv.width) / texture->width,  (uv.y + uv.height) / texture->height, bottom_color.color};
    batch.vertices[3] = Vertex{size.x, 0     , -1.0f, (uv.x + uv.width) / texture->width,  uv.y / texture->height,               top_color.color};
    batch.vertices[4] = batch.vertices[2];
    batch.vertices[5] = batch.vertices[0];
    batch.index_offset = 6;
    batch.end();
}
}
