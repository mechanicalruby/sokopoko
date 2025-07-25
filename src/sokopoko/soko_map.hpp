#ifndef SOKOBAN_MAP
#define SOKOBAN_MAP

#include "../sprite.hpp"
#include "../atlas.hpp"
#include "../render_list.hpp"
#include "soko.hpp"

#include <iostream>
#include <filesystem>
#include <cinttypes>

#include "yyjson/yyjson.h"
#include "nfd.hpp"

namespace fs = std::filesystem;

namespace Sokoban {
struct Tileset {
    Texture* texture;
    size_t starting_index;
};

struct Prop {
    std::string name;
    Turbine::Sprite sprite;
};

struct CameraRegion {
    SokoRegion region;
    Vector2 target;
};

struct Goal {
    uint32_t id; // for which puzzle in the map?
    SokoPosition position;
    bool is_satisfied = false;
};

struct Map {
    std::string name;
    int width, height;
    
    std::vector<uint16_t> tiles;
    std::vector<Tileset> tilemaps;
    std::vector<CameraRegion> cam_regions;
    std::vector<Prop> props;
    std::vector<Goal> goals;
    Turbine::Sprite tile_sprite;

    ObjectList objects;
    // list of controllable characters
    // in Tactics, this will be controlled by game state
    std::array<Sokoban::SokoObject*, 4> c_actors;

    bool is_edit_mode = false;
    bool show_hidden_objects = false;
};

bool load_map(Map& map, const std::string& file_path);
bool save_map(Map& map, const std::string& file_path);
void draw_map(Map& map, Turbine::RenderList& render_list, Turbine::Texture* texture);

void change_tile(Map& map, SokoPosition position, uint16_t new_id);
SokoObject* create_object(Map& map, const std::string& name, SokoObjectClass type, SokoPosition position);
void set_default_object_sprite(SokoObject* obj, Atlas& atlas);

SokoObject* object_at(Map& map, SokoPosition position);
bool attempt_movement(Map& map, SokoObject* actor, SokoPosition destination);

bool check_puzzle_state(Map& map, uint32_t puzzle_id);

// File loading / NFD
std::string save_file_prompt();
std::string load_file_prompt();
}

#endif
