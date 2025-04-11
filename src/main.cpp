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

#include "sokopoko/sokoban.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <cinttypes>
#include <vector>

using Turbine::InputAction;
using Turbine::InputMode;

int main(void) {
    Turbine::Window window {};
    Turbine::init_window(window, "Sokopoko!", 320 * 3, 240 * 3);

    Turbine::InputState input;
    Turbine::init_input_callbacks(input, window);

    if(!Turbine::set_window_icon(window, "./res/icon.png")) {
        printf("WINDOW: Couldn't set window icon.\n");
    }

    Turbine::set_input_bind(input, InputAction::MOVE_UP   , GLFW_KEY_W    , InputMode::MODE_KEY);
    Turbine::set_input_bind(input, InputAction::MOVE_LEFT , GLFW_KEY_A    , InputMode::MODE_KEY);
    Turbine::set_input_bind(input, InputAction::MOVE_DOWN , GLFW_KEY_S    , InputMode::MODE_KEY);
    Turbine::set_input_bind(input, InputAction::MOVE_RIGHT, GLFW_KEY_D    , InputMode::MODE_KEY);
    Turbine::set_input_bind(input, InputAction::UI_UP     , GLFW_KEY_UP   , InputMode::MODE_KEY);
    Turbine::set_input_bind(input, InputAction::UI_LEFT   , GLFW_KEY_LEFT , InputMode::MODE_KEY);
    Turbine::set_input_bind(input, InputAction::UI_DOWN   , GLFW_KEY_DOWN , InputMode::MODE_KEY);
    Turbine::set_input_bind(input, InputAction::UI_RIGHT  , GLFW_KEY_RIGHT, InputMode::MODE_KEY);
    Turbine::set_input_bind(input, InputAction::UNDO      , GLFW_KEY_Z    , InputMode::MODE_KEY);

    Turbine::Shader shader {};
    Turbine::compile_default_shader(shader);
    Turbine::use_shader(shader);
    Turbine::uniform_mat4(shader, "projection", glm::ortho(0.0f, 320.0f, 240.0f, 0.0f, -1.0f, 1.0f));
    Turbine::uniform_mat4(shader, "view", glm::mat4(1.0f));
    Turbine::uniform_mat4(shader, "model", glm::mat4(1.0f));

    Turbine::Texture texture;
    if(!Turbine::load_texture(texture, "./res/objects.png", TB_NEAREST, TB_NEAREST, false)) {
        printf("Didn't load the texture\n");
    }

    Turbine::Texture ui_texture;
    if(!Turbine::load_texture(ui_texture, "./res/ui.png", TB_NEAREST, TB_NEAREST, false)) {
        printf("Didn't load the texture\n");
    }

    Sokoban::ObjectList objects;
    std::vector<Sokoban::Prop>    props;
    std::vector<Turbine::Sprite*> sprites;

    Sokoban::Object player = Sokoban::Object();
    player.sprite.texture = &texture;
    player.sprite.region = Turbine::Rect(0, 41, 28, 43);
    player.sprite.centered = false;
    player.sprite.scale_x = 1.0f;
    player.sprite.scale_y = 1.0f;
    player.position = {0, 0};
    player.is_controlled = true;

    objects.push_back(&player);
    sprites.push_back(&player.sprite);
    
    Sokoban::Object player2 = Sokoban::Object();
    player2.sprite.texture = &texture;
    player2.sprite.region = Turbine::Rect(28, 46, 28, 43);
    player2.sprite.centered = false;
    player2.sprite.scale_x = 1.0f;
    player2.sprite.scale_y = 1.0f;
    player2.position = {3, 3};
    player2.is_controlled = false;
    
    objects.push_back(&player2);
    sprites.push_back(&player2.sprite);

    /*
      struct Object {
      Sprite sprite;
      // object stuff
      }

      struct Prop {
      Sprite sprite;
      // prop stuff
      }

      vector<Object> object_list;
      vector<Prop>   prop_list;
      
      vector<weak_ptr<Sprite>> sprites;
    */

    Turbine::Batch b {};
    Turbine::Batch ub {}; // UI Batch
    Turbine::Batch sb {};
    Turbine::Framebuffer fb(320, 240);
    b.texture  = &texture;
    ub.texture = &ui_texture;
    sb.texture = &fb.texture;

    Turbine::Sprite screen_sprite {};
    screen_sprite.x = (320.0f * 3.0f) / 2.0f;
    screen_sprite.y = (240.0f * 3.0f) / 2.0f;
    screen_sprite.scale_x = 3.0f;
    screen_sprite.scale_y = 3.0f;
    screen_sprite.flip_v = true;
    screen_sprite.region = Turbine::Rect{0.0f, 0.0f, 320.0f, 240.0f};
    screen_sprite.centered = true;
    screen_sprite.color = 0xFFFFFFFF;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.ptr, true);
    ImGui_ImplOpenGL3_Init("#version 150");
    bool show = true;

    Turbine::Camera cam {};
    cam.origin_x = 320.0f / 2.0f;
    cam.origin_y = 240.0f / 2.0f;

    Turbine::RegionTrack trk;
    trk.keys.push_back(Turbine::TypedKey<Turbine::Rect>{0.0, Turbine::Rect{0.0f,  0.0f, 64.0f, 64.0f}});
    trk.keys.push_back(Turbine::TypedKey<Turbine::Rect>{1.0, Turbine::Rect{64.0f, 0.0f, 64.0f, 64.0f}});
    
    while (!Turbine::should_close_window(window)) {
        Turbine::poll_window(window);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for(Sokoban::Object* object : objects) {
            object->sprite.x = object->position.x * 24.0f;
            object->sprite.y = object->position.y * 24.0f;
        }

        Turbine::bind_framebuffer(fb);
        Turbine::uniform_mat4(shader, "projection", glm::ortho(0.0f, 320.0f, 240.0f, 0.0f, 1.0f, -1.0f));
        Turbine::apply_camera(cam, shader);
        Turbine::use_shader(shader);
        
        Turbine::clear(window, 0.6f, 0.6f, 0.6f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        b.begin();
        for(Turbine::Sprite* sprite : sprites) {
            b.queue(*sprite);
        }
        b.draw();

        ub.begin();
        ub.draw();
        
        Turbine::reset_camera(cam, shader);
        Turbine::unbind_texture();
        Turbine::unbind_framebuffer();

        // clear and viewport set
        int fwidth, fheight;
        glfwGetFramebufferSize(window.ptr, &fwidth, &fheight);
        Turbine::set_viewport(0, 0, fwidth, fheight);
        Turbine::uniform_mat4(shader, "projection", glm::ortho(0.0f, (float)fwidth, (float)fheight, 0.0f, 1.0f, -1.0f));
        Turbine::clear(window, 0.0f, 0.0f, 0.0f);

        sb.begin();
        sb.queue(screen_sprite);
        sb.draw();
        
        // ImGui::Begin("Sokopoko!");
        // ImGui::Image((ImTextureID)(intptr_t)fb.texture.id, ImVec2(fb.texture.width * 2.0f, fb.texture.height * 2.0f), ImVec2(0, 1), ImVec2(1, 0));
        // ImGui::End();

        ImGui::Begin("Controls");
        ImGui::SliderFloat("cam.x", &cam.x, -240.0f, 240.0f);
        ImGui::SliderFloat("cam.y", &cam.y, -240.0f, 240.0f);
        ImGui::End();

        ImGui::Begin("Resources");
        
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        Turbine::swap_buffers(window);
    }

    Turbine::destroy_window(window);
    return 0;
}
