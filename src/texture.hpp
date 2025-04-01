#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>
#include <cinttypes>

#define TB_NEAREST GL_NEAREST
#define TB_LINEAR  GL_LINEAR

namespace Turbine {
struct Texture {
    std::string name;
    void* data;
    uint32_t id;
    uint32_t width, height;
    uint32_t filter_min, filter_mag;
    uint32_t wrap_s, wrap_t;
};

bool load_texture(Texture& tex, const std::string& file_path, uint32_t filter_mag, uint32_t filter_min, bool keep_data);
void bind_texture(Texture& tex);
void unbind_texture();

// blank texture target
void generate_texture(Texture& tex, uint32_t width, uint32_t height);
}

#endif
