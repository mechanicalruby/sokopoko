#include "inspector.hpp"

namespace Turbine {
bool imgui_draw_timeline(Animation& anim) {
    if (anim.tracks.empty())
        return false;

    if(!anim.name.empty()) {
        std::string name = "Animation (" + anim.name + ")";
        ImGui::Begin(name.c_str());
    } else {
        ImGui::Begin("Animation");
    }

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
    draw_list->AddRect(canvas_pos, canvas_limit, IM_COL32(17, 17, 17, 255));

    draw_list->PushClipRect(canvas_pos, canvas_limit);

    draw_list->AddLine(ImVec2(canvas_pos.x + 64.0f, canvas_pos.y), ImVec2(canvas_pos.x + 64.0f, canvas_pos.y + canvas_size.y), IM_COL32(55, 55, 55, 255));

    // Todo: SHOW A UNIQUELY COLORED PLAYHEAD FOR EVERY ANIMATION INSTANCE POINTING TO AN ANIMATION RESOURCE

    int i = 0;
    for(Track* track : anim.tracks) {
        switch(track->type) {
        case TrackType::POSITION: {
            PositionTrack* pt = static_cast<PositionTrack*>(track);
            draw_list->AddText(
                ImVec2(canvas_pos.x + 4.0f, canvas_pos.y + 3.0f + (24.0f * i)),
                0xFFFFFFFF,
                "PosTrack"
                );
                
            for(auto key : pt->positions) {
                float x = 72.0f + canvas_pos.x + static_cast<float>(key.time) * (canvas_size.x * 10.0f);
                float y = canvas_pos.y + 12.0f + (24.0f * i);
                float size = 4.0f;

                ImVec2 top(x, y - size);
                ImVec2 right(x + size, y);
                ImVec2 bottom(x, y + size);
                ImVec2 left(x - size, y);
                
                draw_list->AddPolyline(
                    std::vector<ImVec2>{top, right, bottom, left}.data(),
                    4,
                    IM_COL32(255, 255, 0, 255),
                    true,
                    1.5f
                    );
            }
            break;
        }
        case TrackType::REGION: {
            RegionTrack* pt = static_cast<RegionTrack*>(track);
            draw_list->AddText(
                ImVec2(canvas_pos.x + 4.0f, canvas_pos.y + 3.0f + (24.0f * i)),
                0xFFFFFFFF,
                "RegTrack"
                );
                
            for(auto key : pt->regions) {
                float x = 72.0f + canvas_pos.x + static_cast<float>(key.time) * (64.0f);
                float y = canvas_pos.y + 12.0f + (24.0f * i);
                float size = 6.0f;

                ImVec2 c1(x - size, y - size);
                ImVec2 c2(x + size, y + size);
                
                draw_list->AddRect(
                    c1,
                    c2,
                    IM_COL32(255, 144, 60, 255),
                    1.0f
                    );
            }            
            break;
        }
        default: {
            break;
        }
        }

        // track border
        draw_list->AddLine(ImVec2(canvas_pos.x, canvas_pos.y + (24.0f * (i+1))), ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + (24.0f * (i+1))), IM_COL32(55, 55, 55, 255));
        i++;
    }

    // draw_list->AddLine();
    
    ImGui::Dummy(canvas_size); // advance cursor
    ImGui::End();
    return true;
}

void imgui_apply_style() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);

    style.WindowRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.TabRounding = 0.0f;
}
}
