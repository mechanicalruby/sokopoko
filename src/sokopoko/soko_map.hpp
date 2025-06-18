#ifndef SOKOBAN_MAP
#define SOKOBAN_MAP

#include "../sprite.hpp"
#include "soko.hpp"

#include <iostream>
#include <filesystem>
#include <cinttypes>

#include "yyjson/yyjson.h"
#include "nfd.hpp"

namespace fs = std::filesystem;

namespace Sokoban {
struct Tilemap {
    Texture* texture;
    size_t starting_index;
};

struct Map {
    std::string name;
    int width, height;
    
    std::vector<uint16_t> tiles;
    std::vector<Tilemap> tilemaps;
    Turbine::Sprite tile_sprite;

    ObjectList objects;
    // list of controllable characters
    // in Tactics, this will be controlled by game state
    std::array<Sokoban::SokoObject*, 4> c_actors;

    bool is_edit_mode = false;
    bool show_hidden_objects = false;
};

struct Prop {
    std::string name;
    Turbine::Sprite sprite;
};

struct CameraTarget {
    float x, y;
};

struct Scene {
    std::string name;
    Map map;
    CameraTarget camera_target;
};

bool load_map(Map& map, const std::string& file_path);
bool save_map(Map& map, const std::string& file_path);
void draw_map(Map& map, Turbine::Batch& batch);

void change_tile(Map& map, SokoPosition position, uint16_t new_id);
SokoObject* create_object(Map& map, const std::string& name, SokoObjectClass type, SokoPosition position);

SokoObject* object_at(const ObjectList& objects, SokoPosition position);
bool attempt_movement(ObjectList& objects, SokoObject* actor, SokoPosition destination);

// File loading / NFD
std::string save_file_prompt();
std::string load_file_prompt();
}

#endif
