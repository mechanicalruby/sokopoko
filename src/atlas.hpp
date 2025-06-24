#ifndef ATLAS_HPP
#define ATLAS_HPP

#include "sprite.hpp"

#include "yyjson/yyjson.h"

#include <cstdint>
#include <array>
#include <string>
#include <unordered_map>

namespace Turbine {
struct Atlas {
    struct Region {
        std::string name;
        Rect rect;
        Vector2 offset;
        uint8_t tex_id; // for multi-texture atlas..
    };

    std::unordered_map<std::string, Region> regions;
    std::array<Texture*, 32> textures;

    const Region* get_region(const std::string& key) const {
        auto it = regions.find(key);
        if (it != regions.end()) {
            return &it->second;
        }
        return nullptr; // Not found
    }

    void set_region(Sprite& sprite, const std::string& key) {
        if(get_region(key) != nullptr) {
            sprite.region = get_region(key)->rect;
        }
    }
};

bool load_atlas(Atlas& at, const std::string& file_path);
}
#endif
