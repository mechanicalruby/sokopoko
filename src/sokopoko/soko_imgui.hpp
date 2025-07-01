#ifndef SOKOBAN_IMGUI_HPP
#define SOKOBAN_IMGUI_HPP

#include "soko.hpp"
#include "soko_map.hpp"
#include "soko_sky.hpp"

#include "imgui.h"

namespace Sokoban {
void imgui_map_inspect(Map& map, SokoObject*& c_actor, Sky& sky);
void imgui_tile_inspect(Map& map);
}

#endif
