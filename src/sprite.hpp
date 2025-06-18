#ifndef SPRITE_HPP
#define SPRITE_HPP

#define BATCH_SIZE 1536

#include "vbuffer.hpp"
#include "texture.hpp"

#include <iostream>
#include <cinttypes>
#include <cstdio>
#include <string>
#include <array>
#include <vector>

#include <glm/glm.hpp>

namespace Turbine {
struct Vertex {
    float x, y, z;
    float u, v;
    uint32_t color;
};

struct Rect {
    float x, y;
    float width, height;
};

struct Vector2 {
    float x, y;
};

union Color {
    struct {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } rgba;
    uint32_t color;
};

struct Sprite {
    Vector2 position;
    Vector2 origin;
    Vector2 offset;
    Vector2 scale;
    float rotation;
    float layer;
    bool centered;
    bool flip_h;
    bool flip_v;
    bool visible;
    uint32_t color;
    Rect region;
    Texture* texture;

    Sprite();

    // setter/getter
    void set_region(const Rect& value);
    Rect get_region() const;
};

struct Batch {
    std::array<Vertex, BATCH_SIZE> vertices;
    Texture* texture;
#if !TB_GRAPHICS_LEGACY
    uint32_t vbo;
#if !TB_GRAPHICS_GLES2
    uint32_t vao;
#endif
#endif
    
    size_t index_offset;
    bool is_initialized = false;
    
    Batch() = default;
    void initialize(void);
  
    void calculate_vertices(Sprite& sprite, size_t offset);

    void queue(Sprite& sprite);
    void begin(void);
    void end(void);
};

void y_compare_sprites(const Sprite& a, const Sprite& b);
void layer_compare_sprites(const Sprite& a, const Sprite& b);

// automatic batcher
void draw_sprite_list(const std::vector<Sprite>& sprites);
}

#endif
