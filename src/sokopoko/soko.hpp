#ifndef SOKOBAN_HPP
#define SOKOBAN_HPP

#include "../sprite.hpp"
#include "../animation.hpp"
#include "../animation_player.hpp"
#include "../registry.hpp"
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
    NPC_STATIC,
    NPC_FOLLOW
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

enum SokoDirection : uint32_t {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

enum SokoObjectState : bool {
    IDLE,
    IN_TRANSIT
};

// Grid bound game object
struct SokoObject {
    SokoPosition position {};
    SokoObjectBehaviour behaviour = SokoObjectBehaviour::STATIC;
    SokoObjectClass type = SokoObjectClass::BARRIER;
    SokoDirection direction = SokoDirection::SOUTH;
    SokoObjectState state = SokoObjectState::IDLE;
    bool is_controlled = false;
    bool hidden = false;

    // Determines length of player move interpolation in sec.
    double move_speed = 0.1;
    // Normalized value for transitioning to a new position.
    double move_progress = 0.0;

    // Determines NPC_FOLLOW behaviour
    unsigned int party_position = 0;
    
    std::string name;
    Sprite sprite;
};

struct SokoDialogue : public SokoObject {
    std::string tooltip;
    bool auto_trigger = false;
    // fill
};

typedef std::vector<SokoObject> ObjectList;
SokoPosition dir_to_vec(SokoDirection direction);
}

#endif
