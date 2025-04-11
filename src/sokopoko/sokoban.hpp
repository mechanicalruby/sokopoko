#ifndef SOKOBAN_HPP
#define SOKOBAN_HPP

namespace Sokoban {
struct Position {
    int x, y;
};

// Flag for object behavior
enum ObjectType {
    NONE,
    CRATE,
    PLAYER
};

// Grid bound game object
struct Object {    
    Position position;
    ObjectType type;
    bool is_controlled;
    
    std::string name;
    Turbine::Sprite sprite;
};

// Freely placed and Y-sorted scene sprite
struct Prop {
    std::string name;
    Turbine::Sprite sprite;
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

typedef std::vector<Object*> ObjectList;

void draw_object_shadows(const ObjectList& list);
void add_object(ObjectList& list);
};

#endif
