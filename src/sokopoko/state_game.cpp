#include "state_game.hpp"

namespace Sokoban {
void GameState::init() {
    b1.initialize();
    Turbine::load_texture(texture, "./res/objects.png", TB_NEAREST, TB_NEAREST, false);
    b1.texture = &texture;

    sprite.position.x = 6.0f;
    sprite.position.y = 6.0f;
    sprite.centered = false;
    sprite.region = Turbine::Rect{0, 0, 26, 43};
}

void GameState::update(double delta_time, Turbine::InputState& input) {
    if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_UP)) {
        // move item
    }
    if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_DOWN)) {
        // move item
    }
}

void GameState::draw(Turbine::Window& window, Turbine::Shader& base_shader) {
    Turbine::clear(window, 0.0f, 0.0f, 0.0f);
    Turbine::set_blend_mode(window);

    b1.begin();
    b1.queue(sprite);
    b1.end();
}
}
