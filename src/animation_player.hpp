#ifndef ANIMATION_PLAYER_HPP
#define ANIMATION_PLAYER_HPP

#include "animation.hpp"

#include <unordered_map>

namespace Turbine {
struct AnimCollection {
    double current_time = 0.0;
    Animation* current_animation;

    // change to a map of resources later
    std::unordered_map<std::string, Animation> animations;
};

bool add_animation(AnimCollection& ac, Animation& anim);
void play_animation(AnimCollection& ac, const std::string& name);
// void apply_track(Track* track, Property* prop);
void apply_animation(Animation& anim, double current_time);
}
#endif
