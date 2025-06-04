#include "window.hpp"

namespace Turbine {
bool init_window(Window& window, const std::string& name, int width, int height) {
    glfwInit();

#if TB_GRAPHICS_GLES2
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    window.ptr = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (window.ptr == nullptr) {
        printf("Failed to initialize window.\n");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window.ptr);

#if TB_GRAPHICS_GLES2
    int version = gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);
    if(version == 0) {
        printf("Failed to initialize OpenGL ES context.\n");
        return false;
    }
#else
    int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if(version == 0) {
        printf("Failed to initialize OpenGL context.\n");
        return false;
    }
#endif

    glfwSwapInterval(0); // vsync off

    window.width = width;
    window.height = height;
    return true;
}

void set_blend_mode(Window& window) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

bool set_window_icon(Window& window, const std::string& file_path) {
    Turbine::Texture icon;
    
    if(!Turbine::load_texture(icon, file_path, TB_NEAREST, TB_NEAREST, true)) {
        return false;
    }
    
    GLFWimage glfw_icon = GLFWimage {
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

void poll_window(Window& window) {
    glfwPollEvents();
}

void set_viewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void swap_buffers(Window& window) {
    glfwSwapBuffers(window.ptr);
}

void clear(Window& window, float r, float g, float b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
}
