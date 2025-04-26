#include "animation.hpp"

namespace Turbine {
Animation::~Animation() {
    for (int i = 0; i < tracks.size(); i++) {
        delete tracks[i];
    }
}

bool Animation::empty(void) {
    return tracks.empty();
}

void Animation::tick(double delta_time) {
    current_time += 1.0 * delta_time;
    
    for (Track* track : tracks) {
        
    }
}

void Animation::play(void) {
    
}
}
