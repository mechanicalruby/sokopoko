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

void sort_track(Animation& anim, int p_track);
}
#endif
