#include "inspector.hpp"

namespace Turbine {
bool imgui_draw_timeline(Animation& anim, float width, float height) {
    if (anim.empty())
        return false;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos(); // top-left of the canvas
    ImVec2 canvas_size = ImVec2(width, height);
    ImVec2 canvas_end = ImVec2(canvas_pos.x + width, canvas_pos.y + height);

    // Time range (for zooming, etc)
    ImVec2 range = ImVec2(0.0f, 10.0f);

    // Draw background and border
    draw_list->AddRectFilled(canvas_pos, canvas_end, IM_COL32(30, 30, 30, 255));
    draw_list->AddRect(canvas_pos, canvas_end, IM_COL32(255, 255, 255, 255));

    // Time to pixel conversion
    // float time_range = static_cast<float>(maxTime - minTime);
    // auto time_to_x = [&](double t) -> float {
    //     float norm = static_cast<float>((t - minTime) / time_range);
    //     return canvas_pos.x + norm * width;
    // };

    // Draw vertical lines for each second
    // int start_sec = static_cast<int>(std::floor(minTime));
    // int end_sec = static_cast<int>(std::ceil(maxTime));
    // for (int sec = start_sec; sec <= end_sec; ++sec) {
    //     float x = time_to_x(sec);
    //     draw_list->AddLine(ImVec2(x, canvas_pos.y), ImVec2(x, canvas_pos.y + height), IM_COL32(100, 100, 100, 255));
    //     draw_list->AddText(ImVec2(x + 2, canvas_pos.y), IM_COL32_WHITE, std::to_string(sec).c_str());
    // }

    // Draw keyframes as diamonds
    // for (const auto& key : track.keys) {
    //     float x = time_to_x(key.time);
    //     float y = canvas_pos.y + height * 0.5f;
    //     float size = 6.0f;

    //     ImVec2 top(x, y - size);
    //     ImVec2 right(x + size, y);
    //     ImVec2 bottom(x, y + size);
    //     ImVec2 left(x - size, y);

    //     draw_list->AddPolyline(
    //         std::vector<ImVec2>{top, right, bottom, left}.data(),
    //         4,
    //         IM_COL32(255, 255, 0, 255),
    //         true, // closed shape
    //         1.5f
    //         );
    // }

    // float cur_x = time_to_x(4.0);
    // draw_list->AddLine(ImVec2(cur_x, canvas_pos.y), ImVec2(cur_x, canvas_pos.y + height), IM_COL32(100, 0, 0, 255));

    ImGui::Dummy(canvas_size); // advance cursor
    return true;
}

}
