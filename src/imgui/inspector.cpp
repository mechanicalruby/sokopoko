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

    int i = 0;
    for(Track* track : anim.tracks) {
        switch(track->type) {
        case TrackType::POSITION: {
            PositionTrack* pt = static_cast<PositionTrack*>(track);
            draw_list->AddText(
                ImVec2(canvas_pos.x + 4.0f, canvas_pos.y + 3.0f + (24.0f * i)),
                0xFFFFFFFF,
                "position"
                );
                
            for(auto key : pt->positions) {
                float x = 72.0f + canvas_pos.x + static_cast<float>(key.time) * 10.0f;
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
                "region"
                );
                
            for(auto key : pt->regions) {
                float x = 72.0f + canvas_pos.x + static_cast<float>(key.time) * 10.0f;
                float y = canvas_pos.y + 12.0f + (24.0f * i);
                float size = 4.0f;

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
        
        i++;
    }

    // draw_list->AddLine();
    
    ImGui::Dummy(canvas_size); // advance cursor
    ImGui::End();
    return true;
}



}
