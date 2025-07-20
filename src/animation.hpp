#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "sprite.hpp"
#include "registry.hpp"

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

enum TrackType {
    POSITION,
    ROTATION,
    SCALE,
    REGION
};

struct Track {
    std::string target;
    TrackType type;
};

struct PositionTrack : public Track {
    std::vector<TKey<Vector2>> positions;
    PositionTrack() {
        type = TrackType::POSITION;
    }
};

struct ScaleTrack: public Track {
    std::vector<TKey<Vector2>> scales;
    ScaleTrack() {
        type = TrackType::SCALE;
    }
};

struct RegionTrack : public Track {
    std::vector<TKey<Rect>> regions;
    RegionTrack() {
        type = TrackType::REGION;
    }
};

struct Animation {
    std::string name;
    std::vector<Track*> tracks;
    double length = 1.0;
    ~Animation();
};

int add_track(Animation& anim, TrackType type);
void remove_track(Animation& anim, int p_track);

Track* get_track(Animation& anim, int p_track);
void set_track_target(Animation& anim, int p_track, const std::string& property_path);

int position_track_insert_key(Animation& anim, int p_track, double p_time, const Vector2& position);
int scale_track_insert_key(Animation& anim, int p_track, double p_time, const Vector2& scale);
int region_track_insert_key(Animation& anim, int p_track, double p_time, const Rect& region);

/* templates with their definitions here. c++! */

template<typename T> // compare keys to one another
bool key_ascend_sort_comp(const TKey<T>& a, const TKey<T>& b) {
    return a.time < b.time;
}

template<typename T> // compare a key to playhead (for lower_bound)
bool key_playhead_lower_sort_comp(const TKey<T>& a, double t) {
    return a.time < t;
}

template<typename T> // compare a key to playhead (for upper_bound)
bool key_playhead_upper_sort_comp(double t, const TKey<T>& b) {
    return t < b.time;
}

template<typename T>
const TKey<T>& get_lower_bound_key(const std::vector<TKey<T>>& keys, double t) {
    auto key_it = std::lower_bound(keys.begin(), keys.end(), t, key_playhead_lower_sort_comp<T>);
    
    if (key_it == keys.begin()) {
        return *key_it;
    }
    
    --key_it;
    return *key_it;
}

template<typename T>
const TKey<T>& get_upper_bound_key(const std::vector<TKey<T>>& keys, double t) {
    auto key_it = std::upper_bound(keys.begin(), keys.end(), t, key_playhead_upper_sort_comp<T>);
    
    if (key_it == keys.end()) {
        return *key_it;
    }
    
    return *key_it;
}

template<typename T>
const T get_interpolated_value(const std::vector<TKey<T>>& keys, double t) {
    ZoneScoped;
    const TKey<T>& lower_key = Turbine::get_lower_bound_key<T>(keys, t);
    const TKey<T>& upper_key = Turbine::get_upper_bound_key<T>(keys, t);

    // local_lower_key = 0
    double local_upper_key = upper_key.time - lower_key.time;
    double local_playhead  = t - lower_key.time;

    // if they're that close, they're probably the same...
    if (local_upper_key <= 0.000001) {
        return lower_key.value;
    }

    double progress = local_playhead / local_upper_key;

    // now actually perform linear interpolation
    // y0 (x1 - x) + y1 (x - x0) / x1 - x0
    
    T final = lower_key.value * (1.0 - progress) + upper_key.value * progress;
    return final;
}

void sort_track(Animation& anim, int p_track);
}
#endif
