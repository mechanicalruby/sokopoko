#define MINIAUDIO_IMPLEMENTATION
#define NOMINMAX
#include <miniaudio/miniaudio.h>

#include "window.hpp"
#include "texture.hpp"
#include "input.hpp"
#include "shader.hpp"
#include "sprite.hpp"
#include "camera.hpp"
#include "framebuffer.hpp" 
#include "animation.hpp"
#include "state.hpp"
#include "engine_type.hpp"

#include "sokopoko/soko.hpp"
#include "sokopoko/state_menu.hpp"
#include "sokopoko/state_game.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <cinttypes>
#include <vector>

#define USE_IMMEDIATE_MODE FALSE
#define USE_GLES2 FALSE

#define PLATFORM_WIN32 TRUE
#define PLATFORM_MACOS FALSE
#define PLATFORM_LINUX FALSE
#define PLATFORM_PLAYSTATION_VITA FALSE

int main(void) {
    constexpr unsigned int INTERNAL_SCREEN_WIDTH  = 320;
    constexpr unsigned int INTERNAL_SCREEN_HEIGHT = 240;
    unsigned int WINDOW_SCALE = 3;

    Turbine::Registry registry;
    // Example::register_class(registry);

    Example e;
    registry.register_method("set_health", &Example::set_health, e);
    registry.register_method("get_health", &Example::get_health, e);
    
    Turbine::Window window {};
    Turbine::init_window(window, "Sokopoko! (turbine-cpp)", INTERNAL_SCREEN_WIDTH * WINDOW_SCALE,
                         INTERNAL_SCREEN_HEIGHT * WINDOW_SCALE);

    Turbine::InputState input;
    Turbine::init_input_callbacks(input, window);
    Turbine::bind_input_defaults(input);

    if(!Turbine::set_window_icon(window, "./res/icon.png")) {
        printf("WINDOW: Couldn't set window icon.\n");
    }

    Turbine::Shader shader {};
    Turbine::compile_default_shader(shader);
    Turbine::use_shader(shader);    
    Turbine::uniform_mat4(shader, "projection", glm::ortho(0.0f, (float)INTERNAL_SCREEN_WIDTH,
                                                           (float)INTERNAL_SCREEN_HEIGHT, 0.0f, 1.0f, -1.0f));
    Turbine::uniform_mat4(shader, "view", glm::mat4(1.0f));
    Turbine::uniform_mat4(shader, "model", glm::mat4(1.0f));
    
    Turbine::Batch screen_batch {};
    Turbine::Framebuffer fb(INTERNAL_SCREEN_WIDTH, INTERNAL_SCREEN_HEIGHT);
    screen_batch.texture = &fb.texture;
    
    Turbine::Sprite render_target {};
    render_target.position.x = (INTERNAL_SCREEN_WIDTH * WINDOW_SCALE) / 2.0f;
    render_target.position.y = (INTERNAL_SCREEN_HEIGHT * WINDOW_SCALE) / 2.0f;
    render_target.scale.x = static_cast<float>(WINDOW_SCALE);
    render_target.scale.y = static_cast<float>(WINDOW_SCALE);
    render_target.flip_v = true;
    render_target.region = Turbine::Rect{0.0f, 0.0f, (float)INTERNAL_SCREEN_WIDTH, (float)INTERNAL_SCREEN_HEIGHT};
    render_target.centered = true;
    render_target.color = 0xFFFFFFFF; 

    Turbine::StateMachine sm;
    sm.register_state(std::make_unique<Sokoban::MenuState>(), "menu");
    sm.register_state(std::make_unique<Sokoban::GameState>(), "game");
    sm.switch_state("menu");
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplGlfw_InitForOpenGL(window.ptr, true);
    ImGui_ImplOpenGL3_Init();

    while (!Turbine::should_close_window(window)) {
        Turbine::poll_window(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        sm.update(1.0f / 60.0f, input);
        
        Turbine::bind_framebuffer(fb);
        Turbine::uniform_mat4(shader, "projection", glm::ortho(0.0f, (float)INTERNAL_SCREEN_WIDTH,
                                                               (float)INTERNAL_SCREEN_HEIGHT, 0.0f, 1.0f, -1.0f));
        Turbine::use_shader(shader);
        
        sm.draw(window, shader);
        Turbine::unbind_framebuffer();

        // set viewport, draw framebuffer
        int fwidth, fheight;
        glfwGetFramebufferSize(window.ptr, &fwidth, &fheight);
        Turbine::set_viewport(0, 0, fwidth, fheight);
        Turbine::uniform_mat4(shader, "projection", glm::ortho(0.0f, (float)fwidth,
                                                               (float)fheight, 0.0f, 1.0f, -1.0f));
        Turbine::clear(window, 0.0f, 0.0f, 0.0f);

        screen_batch.begin();
        screen_batch.queue(render_target);
        screen_batch.end();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        Turbine::swap_buffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    Turbine::destroy_window(window);
    return 0;
}
