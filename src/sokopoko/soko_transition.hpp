#ifndef SOKO_TRANSITION_HPP
#define SOKO_TRANSITION_HPP

namespace Sokoban {
enum TransitionType {
    TRANS_NONE,
    TRANS_FADE,
    TRANS_WIPE,
    TRANS_CLOUD_WIPE
};

struct Transition {
    TransitionType type;
    double duration = 1.0; // seconds
    double current_time = 1.0;
};

void draw_transition(Transition& state);
}

#endif
