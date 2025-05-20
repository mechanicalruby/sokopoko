#ifndef ENGINE_TYPE_HPP
#define ENGINE_TYPE_HPP

#include <functional>
#include <typeindex>
#include <unordered_map>
#include <any>

namespace Turbine {
template<typename T, typename V>
struct Property {
    uint32_t id;
    std::function<V(const T&)> getter;
    std::function<void(T&, V)> setter;
};

// struct EngineClass {};

struct Registry {
public:
    Registry() = default;

    // Setter with one parameter
    template<typename T, typename V>
    void register_method(const std::string& name, void (T::*func)(V)) {
        
    }

    // Getter with no parameters (const)
    template<typename T, typename R>
    void register_method(const std::string& name, R (T::*func)() const) {

    }

    // Getter with no parameters (non-const)
    template<typename T, typename R>
    void register_method(const std::string& name, R (T::*func)(), T& e) {
        
    }

    // void register_property(const std::string& class_name, const std::string& property_name, )
};
}

#endif
