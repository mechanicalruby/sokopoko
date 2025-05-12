#include "inspector.hpp"

namespace Turbine {
bool imgui_draw_timeline(Animation& anim) {
    if (anim.empty())
        return false;

    ImGui::Begin("Animation");

    if(ImGui::Button("New animation")){

    }

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    ImVec2 canvas_limit = ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y);

    // Time range (for zooming, etc)
    ImVec2 range = ImVec2(0.0f, 10.0f);

    // Draw background and border
    draw_list->AddRectFilled(canvas_pos, canvas_limit, IM_COL32(30, 30, 30, 255));
    draw_list->AddRect(canvas_pos, canvas_limit, IM_COL32(255, 255, 255, 255));

    float x = canvas_pos.x + 13.0f;
    float y = canvas_pos.y + canvas_size.y * 0.5f;
    float size = 4.0f;

    ImVec2 top(x, y - size);
    ImVec2 right(x + size, y);
    ImVec2 bottom(x, y + size);
    ImVec2 left(x - size, y);

    for(Track* track : anim.tracks) {
        draw_list->AddPolyline(
            std::vector<ImVec2>{top, right, bottom, left}.data(),
            4,
            IM_COL32(255, 255, 0, 255),
            true,
            1.5f
            );
    }

    // draw_list->AddLine();
    
    ImGui::Dummy(canvas_size); // advance cursor
    ImGui::End();
    return true;
}



}
