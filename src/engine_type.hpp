#ifndef ENGINE_TYPE_HPP
#define ENGINE_TYPE_HPP

/*
  EngineType implements manual reflection of game classes to expose to the engine
  Registry contains various class definitions.
 */

#include <typeindex>
#include <any>

namespace Turbine {
struct Registry;
// Example class
struct Example {
public:
    Example() {
        health = 30;
    }

    static void register_class(Turbine::Registry& reg) {
        
    };

    int get_health() const { return health; }
    void set_health(int value) { health = value; }
private:
    int health;
};

/* IMPLEMENTATION */

template<typename T, typename V>
struct Property {
    std::string name;
};

struct Registry {
public:
    Registry() = default;

    // Setter with one parameter
    template<typename T, typename V>
    void register_method(const std::string& name, void (T::*func)(V), T& e) {
        (e.*func)(60);
    }

    // Getter with no parameters (const)
    template<typename T, typename R>
    void register_method(const std::string& name, R (T::*func)() const, T& e) {
        R result = (e.*func)();
        printf("Health: %i\n", result);
    }

    // Getter with no parameters (non-const)
    template<typename T, typename R>
    void register_method(const std::string& name, R (T::*func)(), T& e) {
        R result = (e.*func)();
    }
};
}

#endif
