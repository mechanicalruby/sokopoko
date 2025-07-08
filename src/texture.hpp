#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>
#include <cinttypes>

#define TB_NEAREST GL_NEAREST
#define TB_LINEAR  GL_LINEAR
#define TB_KEEP_RAW_TEXTURE_DATA 1
#define TB_DISCARD_RAW_TEXTURE_DATA 0

namespace Turbine {
struct Texture {
    std::string name;
    void* data;
    uint32_t id;
    uint32_t width, height;
    uint32_t filter_min, filter_mag;
    uint32_t wrap_s, wrap_t;
};

bool load_texture(Texture& tex, const std::string& file_path, uint32_t filter_mag, uint32_t filter_min, bool keep_data = TB_DISCARD_RAW_TEXTURE_DATA);
void bind_texture(Texture& tex);
void unbind_texture(void);

// blank texture target
void generate_texture(Texture& tex, uint32_t width, uint32_t height);
}
#endif
