#ifndef STATE_HPP
#define STATE_HPP

#include <string>
#include <memory>
#include "input.hpp"
#include "window.hpp"
#include "shader.hpp"

namespace Turbine {
struct State {
    std::string name;
    virtual void init() = 0;
    virtual void update(double delta_time, Turbine::InputState& input) = 0;
    virtual void draw(Turbine::Window& window, Turbine::Shader& base_shader) = 0;
};

struct StateMachine {
public:
    StateMachine() = default;

    void register_state(std::unique_ptr<State> state, const std::string& name) {
        state->name = name;
        states[name] = std::move(state);
    }

    void switch_state(const std::string& name) {
        if (current_state && current_state->name == name) return; // no switch needed

        auto it = states.find(name);
        if (it != states.end()) {
            current_state = it->second.get();
            current_state->init();
        }
    }

    void update(double delta_time, Turbine::InputState& input) {
        if (current_state) current_state->update(delta_time, input);
    }

    void draw(Turbine::Window& window, Turbine::Shader& base_shader) {
        if (current_state) current_state->draw(window, base_shader);
    }

private:
    std::unordered_map<std::string, std::unique_ptr<State>> states;
    State* current_state = nullptr;
};
}

#endif
