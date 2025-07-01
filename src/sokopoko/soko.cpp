#include "soko.hpp"

namespace Sokoban {
SokoPosition dir_to_vec(SokoDirection direction) {
    switch (direction) {
    case SokoDirection::NORTH: return { 0, -1 };
    case SokoDirection::SOUTH: return { 0,  1 };
    case SokoDirection::EAST:  return { 1,  0 };
    case SokoDirection::WEST:  return {-1,  0 };
    default: return {0, 0};
    }
}
}
