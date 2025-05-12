#include "animation.hpp"

namespace Turbine {
Animation::~Animation() {
    for (int i = 0; i < tracks.size(); i++) {
        delete tracks[i];
    }
}

Track* Animation::add_track(TrackType type) {
    Track* track = nullptr;
    
    switch(type) {
    case TrackType::TYPE_POSITION:
        track = new PositionTrack();
    case TrackType::TYPE_REGION:
        track = new RegionTrack();
    }

    if(track != nullptr)
        tracks.push_back(track);

    return track;
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
