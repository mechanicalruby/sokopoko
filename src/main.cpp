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
#include "registry.hpp"
#include "timer.hpp"

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

int main(int argc, char *argv[]) {
    constexpr unsigned int INTERNAL_SCREEN_WIDTH  = 400;
    constexpr unsigned int INTERNAL_SCREEN_HEIGHT = 240;
    unsigned int WINDOW_SCALE = 3;
    
    Turbine::Window window {};
    Turbine::init_window(window, "Sokopoko", INTERNAL_SCREEN_WIDTH * WINDOW_SCALE,
                         INTERNAL_SCREEN_HEIGHT * WINDOW_SCALE);

    Turbine::InputState input;
    Turbine::init_input_callbacks(input, window);
    Turbine::bind_input_defaults(input);

    if(!Turbine::set_window_icon(window, "./res/icon.png")) {
        printf("WINDOW: Couldn't set window icon.\n");
    }

#if !TB_GRAPHICS_LEGACY
    Turbine::Shader shader {};
#if TB_GRAPHICS_GLES2
    Turbine::load_shader_from_file(shader, "./res/shaders/std_gles.vert", "./res/shaders/std_gles.frag");
#else
    Turbine::load_shader_from_file(shader, "./res/shaders/std.vert", "./res/shaders/std.frag");
#endif
    Turbine::use_shader(shader);
    Turbine::uniform_mat4(shader, "projection", glm::ortho(0.0f, (float)INTERNAL_SCREEN_WIDTH,
                                                           (float)INTERNAL_SCREEN_HEIGHT, 0.0f, 1.0f, -1.0f));
    Turbine::uniform_mat4(shader, "view", glm::mat4(1.0f));
    Turbine::uniform_mat4(shader, "model", glm::mat4(1.0f));
#endif
    
    Turbine::Batch screen_batch {};
    screen_batch.initialize();
    Turbine::Framebuffer fb(512, 512); // power of 2
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

    Turbine::StateMachine sm {};
    sm.register_state(std::make_unique<Sokoban::MenuState>(), "menu");
    sm.register_state(std::make_unique<Sokoban::GameState>(), "game");
    sm.switch_state("menu");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsClassic(&ImGui::GetStyle());

    ImGui_ImplGlfw_InitForOpenGL(window.ptr, true);
    ImGui_ImplOpenGL3_Init("#version 100"); // 100 for gles

    double gTime = 0.0f;
    double dt = 0.0f;
    while (!Turbine::should_close_window(window)) {
        Turbine::poll_window(window);
        calculate_delta_time(dt);
    	gTime += dt;
	
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        sm.update(dt, input);
        
    	Turbine::bind_framebuffer(fb);
        Turbine::set_viewport(0, 0, INTERNAL_SCREEN_WIDTH, INTERNAL_SCREEN_HEIGHT);
#if !TB_GRAPHICS_LEGACY
        Turbine::uniform_mat4(shader, "projection", glm::ortho(0.0f, (float)INTERNAL_SCREEN_WIDTH,
                                                               (float)INTERNAL_SCREEN_HEIGHT, 0.0f, 1.0f, -1.0f));
        Turbine::uniform_mat4(shader, "view", glm::mat4(1.0f));
        Turbine::uniform_mat4(shader, "model", glm::mat4(1.0f));

        Turbine::use_shader(shader);
#endif
        Turbine::clear(window, 0.0f, 0.0f, 0.0f);

        sm.draw(window, shader);
        Turbine::unbind_framebuffer();

        // set viewport, draw framebuffer
        int fwidth, fheight;
        glfwGetFramebufferSize(window.ptr, &fwidth, &fheight);
        Turbine::set_viewport(0, 0, fwidth, fheight);
        Turbine::uniform_mat4(shader, "projection", glm::ortho(0.0f, (float)fwidth,
                                                               (float)fheight, 0.0f, 1.0f, -1.0f));
        Turbine::uniform_float(shader, "uTime", static_cast<float>(gTime));
        Turbine::clear(window, 0.0f, 0.0f, 0.0f);
       
        // render quad to screen
        screen_batch.begin();
        screen_batch.queue(render_target);
        screen_batch.end();

        ImGui::Render();
        ImGui::EndFrame();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        Turbine::swap_buffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    Turbine::destroy_window(window);
    return 0;
}
