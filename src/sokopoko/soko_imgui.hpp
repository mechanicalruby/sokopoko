#ifndef SOKOBAN_IMGUI_HPP
#define SOKOBAN_IMGUI_HPP

#include "soko.hpp"
#include "soko_map.hpp"
#include "soko_sky.hpp"

#include "imgui.h"

namespace Sokoban {
void imgui_map_inspect(Map& list, SokoObject*& c_actor, Sky& sky);
}

#endif
