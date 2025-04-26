#ifndef SOKOBAN_HPP
#define SOKOBAN_HPP

#include "../sprite.hpp"
#include "../animation.hpp"
#include "../engine_type.hpp"
#include <string>

using namespace Turbine;

namespace Sokoban {
struct SokoPosition {
    int x, y;
};

// Flag for object class, determines actual game behavior
enum SokoObjectType {
    NONE,
    CRATE,
    PLAYER
};

// Grid bound game object
struct SokoObject {
    SokoPosition position {};
    SokoObjectType type = SokoObjectType::NONE;
    bool is_controlled = false;
    
    std::string name;
    Sprite sprite;
};

/* OBJECTS */
struct Ross : public SokoObject {
    Animation step;
    
    Ross(SokoPosition pos) {
        type = SokoObjectType::PLAYER;
        position = pos;
        sprite.region = Rect{0, 0, 14, 22};
        name = "Player";
    }
};

struct Crate : public SokoObject {
    Crate(SokoPosition pos) {
        type = SokoObjectType::CRATE;
        position = pos;
        sprite.region = Rect{52, 17, 22, 29};
        sprite.offset.y = 6;
        name = "Crate";
    }
};

/* MAP */
// Freely placed and Y-sorted scene sprite
struct Prop {
    std::string name;
    Sprite sprite;
};

// Rendered tilemap / scene base
struct Map {
    std::string name;
    std::array<uint32_t, 4096> tiles;
};

// Scene stuff
struct CameraTarget {
    float x, y;
};

struct Scene {
    std::string name;
    Map map;
    CameraTarget camera_target;
};

typedef std::vector<SokoObject*> ObjectList;

void draw_object_shadows(const ObjectList& list);
void add_object(ObjectList& list);
};

#endif
