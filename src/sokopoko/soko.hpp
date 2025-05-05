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
    PLAYER,
    BARRIER,
    
};

// Grid bound game object
struct SokoObject {
    SokoPosition position {};
    SokoObjectType type = SokoObjectType::NONE;
    bool is_controlled = false;
    bool hidden = false;
    
    std::string name;
    Sprite sprite;
};

/* OBJECTS */
struct Ross : public SokoObject {
    Animation step;
    
    Ross(SokoPosition pos) {
        type = SokoObjectType::PLAYER;
        position = pos;
        sprite.region = Rect{0, 170, 32, 43};
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

struct Barrier : public SokoObject {
    Barrier(SokoPosition pos) {
        type = SokoObjectType::BARRIER;
        hidden = true;
        position = pos;
        sprite.region = Rect{56, 75, 22, 29};
        sprite.offset.y = 6;
        name = "Barrier";
    }
};

typedef std::vector<SokoObject*> ObjectList;

void draw_object_shadows(const ObjectList& list);
void add_object(ObjectList& list);
};

#endif
