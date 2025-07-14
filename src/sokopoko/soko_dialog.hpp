#ifndef SOKO_DIALOG_HPP
#define SOKO_DIALOG_HPP

#include "sprite.hpp"
#include "atlas.hpp"
#include "animation.hpp"
#include "animation_player.hpp"

namespace Sokoban {
enum DialogEnterEffect {
    ENTER_NONE,
    ENTER_SHAKE,
    ENTER_FADE,
    ENTER_FADE_AND_SLIDE
};

struct DialogState {
    std::string display;
    std::array<std::string, 32> buffer;
    DialogEnterEffect effect;
    
    double progress = 0.0f; // 0.0->1.0
    double speed = 0.0f;
    bool is_active = false;

    uint8_t buffer_index = 0;
    size_t buffer_length = 0;
};

void display_default_dialog(Turbine::Batch& batch, Turbine::Atlas& at, DialogState& ds);
}

#endif
