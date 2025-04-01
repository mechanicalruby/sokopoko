#include "window.h"

namespace Turbine {
bool init_window(Window& window, const std::string& name, int width, int height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window.ptr = glfwCreateWindow(width, height, name, NULL, NULL);
    if (window.ptr == nullptr) {
        printf("Failed to initialize window.");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window.ptr);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1); // vsync on

    window.width = width;
    window.height = height;
    return true;
}

bool set_window_icon(Window& window, const std::string& file_path) {
    Turbine::Texture icon;
    
    if(!Turbine::load_texture(&icon, file_path, TB_NEAREST, TB_NEAREST, true)) {
        return false;
    }
    
    GLFWimage glfw_icon = (GLFWimage){
        .width = (int)icon.width,
        .height = (int)icon.height,
        .pixels = (unsigned char*)icon.data,
    };
    
    glfwSetWindowIcon(window.ptr, 1, &glfw_icon);
    return true;
}

bool should_close_window(Window& window) {
    return glfwWindowShouldClose(window.ptr);
}

bool destroy_window(Window& window) {
    glfwDestroyWindow(window.ptr);
    glfwTerminate();
    window.ptr = nullptr;

    if(window.ptr == nullptr) {
        return true;
    }
    return false;
}
}