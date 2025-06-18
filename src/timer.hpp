#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

namespace Turbine {
inline void calculate_delta_time(double& delta_time) {
    using clock = std::chrono::high_resolution_clock;
    static auto last_time = clock::now();

    auto current_time = clock::now();
    std::chrono::duration<double> elapsed = current_time - last_time;
    delta_time = elapsed.count();
    last_time = current_time;
}
}

#endif
