#ifndef SOKOBAN_HPP
#define SOKOBAN_HPP

#include "../sprite.hpp"
#include "../animation.hpp"
#include "../animation_player.hpp"
#include "../engine_type.hpp"
#include <string>

using namespace Turbine;

namespace Sokoban {
struct SokoPosition {
    int x, y;
};

struct SokoRegion {
    int x, y;
    int width, height;
};

// Generic object behaviour flags.
// Determines game behaviour for functionally unique objects.
enum SokoObjectBehaviour : uint32_t {
    STATIC,
    CRATE,
    DOOR,
    GOAL,
    PLAYER,
    NPC
};

// Specific object definition flags.
// Determines object rendering, effects.
enum SokoObjectClass : uint32_t {
    BARRIER,
    ROSS,
    MIRAGE,
    WOODEN_CRATE,
    METAL_CRATE,
    PILLOW,
    PILLOW_BLUE,
    PILLOW_RED
};

// Grid bound game object
struct SokoObject {
    SokoPosition position {};
    SokoObjectBehaviour behaviour = SokoObjectBehaviour::STATIC;
    SokoObjectClass type = SokoObjectClass::BARRIER;
    bool is_controlled = false;
    bool hidden = false;
    
    std::string name;
    Sprite sprite;
};

struct SokoDialogue : public SokoObject {
    std::string tooltip;
    bool auto_trigger = false;
    // fill
};

/* OBJECTS */
struct Ross : public SokoObject {
    AnimCollection ac;
    
    Ross(SokoPosition pos) {
        type = SokoObjectClass::ROSS;
        behaviour = SokoObjectBehaviour::PLAYER;
        position = pos;
        sprite.region = Rect{0, 0, 32, 42};
        // sprite.region = Rect{32, 175, 32, 43};
        name = "Player";

        // play an animation
        Turbine::play_animation(ac, "move_s");
    }
};

struct Mirage : public SokoObject {
    AnimCollection ac;
    
    Mirage(SokoPosition pos) {
        type = SokoObjectClass::MIRAGE;
        behaviour = SokoObjectBehaviour::NPC;
        position = pos;
        sprite.region = Rect{0, 0, 32, 42};
        // sprite.region = Rect{32, 175, 32, 43};
        name = "Mirage";
    }
};

struct Crate : public SokoObject {
    Crate(SokoPosition pos) {
        type = SokoObjectClass::WOODEN_CRATE;
        behaviour = SokoObjectBehaviour::CRATE;
        position = pos;
        sprite.region = Rect{22, 84, 22, 29};
        sprite.offset.y = 6;
        name = "Crate";
    }
};

struct Barrier : public SokoObject {
    Barrier(SokoPosition pos) {
        type = SokoObjectClass::BARRIER;
        behaviour = SokoObjectBehaviour::STATIC;
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
}

#endif
