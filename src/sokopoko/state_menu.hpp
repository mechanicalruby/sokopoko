#ifndef SOKO_MENU_STATE_HPP
#define SOKO_MENU_STATE_HPP

#include "../window.hpp"
#include "../sprite.hpp"
#include "../string.hpp"
#include "../state.hpp"
#include "../meta.hpp"
#include "../animation.hpp"
#include "../render_list.hpp"
#include "../camera.hpp"

#include "sokoban.hpp"

#include "imgui.h"

namespace Sokoban {
struct MenuState : public Turbine::State {
private:
    Turbine::Batch b1;
    Turbine::Batch b2;
    Turbine::Texture texture;
    Turbine::Texture object_texture;

    Turbine::Sprite sprite {};
    Turbine::Animation anim {};

    Turbine::Camera cam;
    ObjectList objects;

    bool camera_follow_player = false;
public:
    MenuState() = default;
    ~MenuState();
    void init() override;
    void update(double delta_time, Turbine::InputState& input) override;
    void draw(Turbine::Window& window, Turbine::Shader& base_shader) override;
};
}

#endif
