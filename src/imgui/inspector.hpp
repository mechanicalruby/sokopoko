#ifndef IMGUI_INSPECTOR_HPP
#define IMGUI_INSPECTOR_HPP

/*
  Custom Sokopoko/Turbine widgets for debugging
 */

#include "../animation.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Turbine {

/* INTERNAL */


/* GENERAL */

/* ANIMATION */

bool imgui_draw_timeline(Animation& anim, float width = 600.0f, float height = 100.0f);

}

#endif
