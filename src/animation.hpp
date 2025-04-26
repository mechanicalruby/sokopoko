#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "sprite.hpp"
#include <cassert>
#include <algorithm>
#include <unordered_map>

namespace Turbine {
struct Key {
    double time = 0.0;
};

template<typename T>
struct TKey : public Key {
    T value;
};

enum TrackType : uint8_t {
    TYPE_POSITION,
    TYPE_ROTATION,
    TYPE_SCALE,
    TYPE_REGION,
    TYPE_AUDIO,
    TYPE_ANIMATION,
};

enum InterpolationType : uint8_t {
    NEAREST,
    LINEAR
};

struct Track {
    TrackType type = TrackType::TYPE_ANIMATION;
    InterpolationType interpolation = InterpolationType::NEAREST;
};

struct PositionTrack : public Track {
    std::vector<TKey<Vector2>> keys;

    PositionTrack() {
        type = TrackType::TYPE_POSITION;
    }
};

struct RegionTrack : public Track {
    std::vector<TKey<Rect>> keys;

    RegionTrack() {
        type = TrackType::TYPE_REGION;
    }
};

struct Animation {
public:
    Animation() = default;
    ~Animation();

    void tick(double delta_time);
    void play();

    /* properties */
    double get_current_time() { return current_time; }
    bool empty();
private:
    std::vector<Track*> tracks;
    double current_time = 0.0;
    double length = 1.0;
};

struct AnimationPlayer {
public:
    AnimationPlayer() = default;
    void add_animation(const std::string& name);
    
    void play(const std::string& name);
private:
    std::unordered_map<std::string, Animation> animations;
};
}
#endif
