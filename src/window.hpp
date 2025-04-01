#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

#include "texture.hpp"

namespace Turbine {
struct Window {
    GLFWwindow* ptr;
    int width, height;
    int scale;
};

bool init_window(Window& window, const std::string& name, int width, int height);
void set_window_scale(Window& window, int scale);
bool set_window_icon(Window& window, const std::string& file_path);
bool should_close_window(Window& window);
bool destroy_window(Window& window);
}
#endif