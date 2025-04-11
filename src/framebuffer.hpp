#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.hpp"

namespace Turbine {
struct Framebuffer {
    Texture texture;
    uint32_t width, height;
    uint32_t id;

    Framebuffer(uint32_t width, uint32_t height);
    ~Framebuffer(void);
};

void bind_framebuffer(Framebuffer& fbuf);
void unbind_framebuffer(void);
}

#endif
