#ifndef ANIMATION_PLAYER_HPP
#define ANIMATION_PLAYER_HPP

#include "animation.hpp"

#include <unordered_map>

namespace Turbine {
// Instances are for animation playback.
// An object wanting to play an animation will get one of these in a queue.
// This allows for generic, global animations that can be stacked on objects.

// e.g. a flashing animation for taking damage may be played at the same time
// as another object-specific animation (like falling over) without requiring
// two players.
struct AnimationInstance {
    // This should be the target object.
    void* ptr;
    // This should be a handle to the game's resource.
    Animation* anim;
    std::string target;
    double current_time = 0.0;
    double speed = 1.0;
    bool finished = false;

    void update(double delta_time) {
        current_time += delta_time;
        if(current_time >= anim->length) {
            finished = true;
        }
    }

    void reset() {
        current_time = 0.0;
        finished = false;
    }
};

// unordered_map binds..
}
#endif
