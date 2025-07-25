#include "input.hpp"

namespace Turbine {
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    InputState* state = (InputState*)glfwGetWindowUserPointer(window);

    for(unsigned int i = 0; i < state->binds_count; i++) {
        if(key == state->binds[i].button && state->binds[i].mode == 0) {
            if(action == GLFW_PRESS) {
                state->pressed_keys[state->binds[i].action] = true;
                state->just_pressed_keys[state->binds[i].action] = true;
            }
            if(action == GLFW_RELEASE) {
                state->pressed_keys[state->binds[i].action] = false;
                state->just_pressed_keys[state->binds[i].action] = false;
            }
        }
    }
}

void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    InputState* state = (InputState*)glfwGetWindowUserPointer(window);
    state->mouse_x = xpos;
    state->mouse_y = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    InputState* state = (InputState*)glfwGetWindowUserPointer(window);

    for(unsigned int i = 0; i < state->binds_count; i++) {
        if(button == state->binds[i].button && state->binds[i].mode == 1) {
            if(action == GLFW_PRESS) {
                state->pressed_keys[state->binds[i].action] = true;
                state->just_pressed_keys[state->binds[i].action] = true;
            }
            if(action == GLFW_RELEASE) {
                state->pressed_keys[state->binds[i].action] = false;
                state->just_pressed_keys[state->binds[i].action] = false;
            }
        }
    }
}

void init_input_callbacks(InputState& state, Window& window) {
    glfwSetWindowUserPointer(window.ptr, &state); // Add state to window user pointer
    glfwSetKeyCallback(window.ptr, key_callback); // Keyboard
    glfwSetCursorPosCallback(window.ptr, mouse_cursor_callback); // Cursor
    glfwSetMouseButtonCallback(window.ptr, mouse_button_callback); // Mouse buttons
    state.binds_count = 0; // init to 0
    state.mouse_x = 0;
    state.mouse_y = 0;
}

bool is_pressed(InputState& state, InputAction action) {
    if(state.pressed_keys[action] == true) {
        return true;
    }

    return false;
}

bool just_pressed(InputState& state, InputAction action) {
    if(state.just_pressed_keys[action] == true) {
        state.just_pressed_keys[action] = false;
        return true;
    }

    return false;
}

bool set_input_bind(InputState& state, InputAction action, unsigned int key, int mode) {
    if(state.binds_count <= 255) {
        state.binds[state.binds_count].button = key;
        state.binds[state.binds_count].action = action;
        state.binds[state.binds_count].mode = (InputMode)mode;
        state.binds_count++;
        return true;
    }
    return false;
}

void bind_input_defaults(InputState& state) {
    // PC, Sokopoko
    Turbine::set_input_bind(state, InputAction::MOVE_UP   , GLFW_KEY_W    , InputMode::MODE_KEY);
    Turbine::set_input_bind(state, InputAction::MOVE_LEFT , GLFW_KEY_A    , InputMode::MODE_KEY);
    Turbine::set_input_bind(state, InputAction::MOVE_DOWN , GLFW_KEY_S    , InputMode::MODE_KEY);
    Turbine::set_input_bind(state, InputAction::MOVE_RIGHT, GLFW_KEY_D    , InputMode::MODE_KEY);
    Turbine::set_input_bind(state, InputAction::UI_UP     , GLFW_KEY_UP   , InputMode::MODE_KEY);
    Turbine::set_input_bind(state, InputAction::UI_LEFT   , GLFW_KEY_LEFT , InputMode::MODE_KEY);
    Turbine::set_input_bind(state, InputAction::UI_DOWN   , GLFW_KEY_DOWN , InputMode::MODE_KEY);
    Turbine::set_input_bind(state, InputAction::UI_RIGHT  , GLFW_KEY_RIGHT, InputMode::MODE_KEY);
    Turbine::set_input_bind(state, InputAction::UNDO      , GLFW_KEY_Z    , InputMode::MODE_KEY);
    // PC, Sokopoko debug
    Turbine::set_input_bind(state, InputAction::EDITOR_PLACE_TILE, GLFW_MOUSE_BUTTON_LEFT, InputMode::MODE_MOUSE);
}
}
