#include "animation.hpp"

namespace Turbine {
Animation::~Animation() {
    for(int i = 0; i < tracks.size(); i++) {
        delete tracks[i];
    }
    tracks.clear();
}

int add_track(Animation& anim, TrackType type) {
    int p_pos = static_cast<int>(anim.tracks.size());
    
    switch(type) {
    case TrackType::POSITION: {
        PositionTrack* pt = new PositionTrack();
        anim.tracks.push_back(pt);
        break;
    }
    case TrackType::SCALE: {
        ScaleTrack* st = new ScaleTrack();
        anim.tracks.push_back(st);
        break;
    }
    case TrackType::REGION: {
        RegionTrack* rt = new RegionTrack();
        anim.tracks.push_back(rt);
        break;
    }
    default: {
        break;
    }
    }
    
    return p_pos;
}

void remove_track(Animation& anim, int p_track) {
    if(p_track < 0 || p_track >= anim.tracks.size()) {
        return;
    }
    
    Track* t = anim.tracks[p_track];

    switch(t->type) {
    case TrackType::POSITION: {
        PositionTrack* pt = static_cast<PositionTrack*>(t);
        pt->positions.clear();
        break;
    }
    case TrackType::SCALE: {
        ScaleTrack* st = static_cast<ScaleTrack*>(t);
        st->scales.clear();
        break;
    }
    case TrackType::REGION: {
        RegionTrack* rt = static_cast<RegionTrack*>(t);
        rt->regions.clear();
        break;
    }
    default: {
        break;
    }
    }
    
    delete t;
    anim.tracks.erase(anim.tracks.begin() + p_track);
}

Track* get_track(Animation& anim, int p_track) {
    if(p_track < 0 || p_track >= anim.tracks.size()) {
        return nullptr;
    }

    return anim.tracks[p_track];
}

void set_track_target(Animation& anim, int p_track, const std::string& property_path) {
    if(p_track < 0 || p_track >= anim.tracks.size()) {
        return;
    }

    anim.tracks[p_track]->target = property_path;
}

int position_track_insert_key(Animation& anim, int p_track, double p_time, const Vector2& position) {
    if(p_track < 0 || p_track >= anim.tracks.size()) {
        return -1;
    }

    if(anim.tracks[p_track]->type != TrackType::POSITION) {
        return -1;
    }

    PositionTrack* pt = static_cast<PositionTrack*>(anim.tracks[p_track]);
    pt->positions.push_back(TKey<Vector2>{p_time, position});

    sort_track(anim, p_track);
    return 0;
}

int scale_track_insert_key(Animation& anim, int p_track, double p_time, const Vector2& scale) {
    if (p_track < 0 || p_track >= anim.tracks.size()) {
        return -1;
    }

    if (anim.tracks[p_track]->type != TrackType::SCALE) {
        return -1;
    }

    ScaleTrack* pt = static_cast<ScaleTrack*>(anim.tracks[p_track]);
    pt->scales.push_back(TKey<Vector2>{p_time, scale});

    sort_track(anim, p_track);
    return 0;
}

int region_track_insert_key(Animation& anim, int p_track, double p_time, const Rect& region) {
    if(p_track < 0 || p_track >= anim.tracks.size()) {
        return -1;
    }

    if(anim.tracks[p_track]->type != TrackType::REGION) {
        return -1;
    }

    RegionTrack* rt = static_cast<RegionTrack*>(anim.tracks[p_track]);
    rt->regions.push_back(TKey<Rect>{p_time, region});

    sort_track(anim, p_track);
    return 0;
}

void sort_track(Animation& anim, int p_track) {
    Track* t = anim.tracks[p_track];

    if(t == nullptr) {
        return;
    }

    switch(t->type) {
    case TrackType::POSITION: {
        PositionTrack* pt = static_cast<PositionTrack*>(t);
        std::sort(pt->positions.begin(), pt->positions.end(), key_ascend_sort_comp<Vector2>);
        break;
    }
    case TrackType::SCALE: {
        ScaleTrack* st = static_cast<ScaleTrack*>(t);
        std::sort(st->scales.begin(), st->scales.end(), key_ascend_sort_comp<Vector2>);
        break;
    }
    case TrackType::REGION: {
        RegionTrack* rt = static_cast<RegionTrack*>(t);
        std::sort(rt->regions.begin(), rt->regions.end(), key_ascend_sort_comp<Rect>);
        break;
    }
    default: {
        break;
    }
    }
}
}
