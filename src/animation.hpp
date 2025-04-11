#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "sprite.hpp"

namespace Turbine {

// Keyframes

struct Key {
    double time = 0.0;
};

template<typename T>
struct TypedKey : public Key {
    T data;
};

// Tracks

enum TrackType {
    POSITION,
    ROTATION,
    SCALE,
    SUBTEXTURE,
    AUDIO,
    ANIMATION
};

enum InterpolationType {
    NEAREST,
    LINEAR
};

struct Track {
    TrackType type = TrackType::ANIMATION;
    InterpolationType interpolation = InterpolationType::NEAREST;
};

struct RegionTrack : public Track {
    std::vector<TypedKey<Rect>> keys;
};


}

#endif
