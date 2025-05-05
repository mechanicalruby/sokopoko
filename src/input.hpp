#ifndef INPUT_HPP
#define INPUT_HPP

#include <cinttypes>

#include "window.hpp"

namespace Turbine {
enum InputAction {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    UNDO,
    RESET,
    INTERACT,

    UI_LEFT,
    UI_RIGHT,
    UI_UP,
    UI_DOWN,

    EDITOR_PLACE_TILE,
    
    ACTION_MAX
};

enum InputMode {
    MODE_KEY,
    MODE_MOUSE
};

struct InputBind {
    unsigned int button;
    InputAction action;
    InputMode mode;
};

struct InputState {
    bool pressed_keys[ACTION_MAX];
    bool just_pressed_keys[ACTION_MAX];
    uint8_t binds_count;
    InputBind binds[255];
    
    double mouse_x, mouse_y;
};

void init_input_callbacks(InputState& state, Window& window);
void bind_input_defaults(InputState& state);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

bool set_input_bind(InputState& state, InputAction action, unsigned int key, int mode);
bool is_pressed(InputState& state, InputAction action);
bool just_pressed(InputState& state, InputAction action);
}
#endif
