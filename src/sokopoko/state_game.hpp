#ifndef SOKO_GAME_STATE_HPP
#define SOKO_GAME_STATE_HPP

#include "../window.hpp"
#include "../sprite.hpp"
#include "../string.hpp"
#include "../state.hpp"
#include "../meta.hpp"
#include "../render_list.hpp"

#include "soko.hpp"

namespace Sokoban {
struct GameState : public Turbine::State {
private:
    Turbine::Batch b1;
    Turbine::Sprite sprite {};
    Turbine::Texture texture;

    ObjectList objects;
public:
    GameState() = default;
    void init() override;
    void update(double delta_time, Turbine::InputState& input) override;
    void draw(Turbine::Window& window, Turbine::Shader& base_shader) override;
};
}

#endif
