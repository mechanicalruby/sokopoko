#ifndef SOKO_MENU_STATE_HPP
#define SOKO_MENU_STATE_HPP

#include "../window.hpp"
#include "../sprite.hpp"
#include "../string.hpp"
#include "../state.hpp"
#include "../render_list.hpp"
#include "../camera.hpp"
#include "../animation.hpp"
#include "../animation_player.hpp"
#include "../atlas.hpp"
#include "../imgui/inspector.hpp"

#include "soko.hpp"
#include "soko_sky.hpp"
#include "soko_map.hpp"
#include "soko_imgui.hpp"

namespace Sokoban {
struct MenuState : public Turbine::State {
private:
    Turbine::Batch b1;
    Turbine::Batch b2;
    Turbine::Batch b3;
    Turbine::Texture texture;
    Turbine::Texture object_texture;
    Turbine::Texture map_texture;

    Turbine::Sprite shadow_sprite {};
    Turbine::Sprite object_preview_sprite {};

    Turbine::Atlas atlas;
    
    Turbine::Shader f_shad {};
    Turbine::Camera cam {};

    Turbine::Animation example_animation {};
    Sokoban::Sky sky {};
    Sokoban::SokoObject* c_actor = nullptr; // controlled actor
    std::vector<Turbine::Sprite*> sprite_list;
    std::vector<AnimationInstance> animation_list;
    Map map;

    double mouse_x, mouse_y;
    double current_time = 0.0f;

    SokoObject* find_player(const ObjectList& list);
    void move_player();
public:
    MenuState() = default;
    ~MenuState();
    void init() override;
    void update(double delta_time, Turbine::InputState& input) override;
    void draw(Turbine::Window& window, Turbine::Shader& base_shader) override;
};
}

#endif
