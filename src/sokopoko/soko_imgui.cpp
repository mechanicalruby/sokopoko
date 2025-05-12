#include "soko_imgui.hpp"

namespace Sokoban {
void imgui_map_inspect(Map& map) {
    ImGui::Begin("Map");

    if(ImGui::Button("Load map")) {
        load_map(map, load_file_prompt());
    }
    ImGui::SameLine();
    if(ImGui::Button("Save map")) {
        save_map(map, save_file_prompt());
    }

    ImGui::InputInt("Width", &map.width);
    ImGui::InputInt("Height", &map.height);

    ImGui::Separator(); // Settings

    ImGui::Checkbox("Edit mode", &map.is_edit_mode);
    ImGui::Checkbox("Show hidden objects", &map.show_hidden_objects);

    ImGui::Separator(); // Objects

    if(ImGui::Button("Create object")) {
        create_object(map, SokoObjectClass::WOODEN_CRATE, SokoPosition{2, 2});
    }

    for(int i = 0; i < map.objects.size(); i++) {
        SokoObject* object = map.objects[i];

        if(object->hidden && !map.show_hidden_objects)
            continue;

        ImGui::PushID(object);
        if (ImGui::TreeNode(object->name.c_str())) {
            ImGui::Text("Position: %i, %i", object->position.x, object->position.y);
            ImGui::Checkbox("Hidden", &object->hidden);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    
    ImGui::End();

    // Tilemap editor

    ImGui::Begin("Object palette");

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 window_pos = ImGui::GetCursorScreenPos();
    ImVec2 window_size = ImGui::GetContentRegionAvail();
    
    float scroll_x = ImGui::GetScrollX();
    float scroll_y = ImGui::GetScrollY();

    draw_list->PushClipRect(window_pos, ImVec2(window_pos.x + window_size.x,
                                               window_pos.y + window_size.y));

    ImVec2 size = ImVec2(window_pos.x + 256*2, window_pos.y + 256*2);

    if(!map.objects.empty()) {
        if(map.objects[0]->sprite.texture != nullptr) {
            draw_list->AddImage((ImTextureID)map.objects[0]->sprite.texture->id, window_pos, size);
        }
    }
    
    ImGui::End();

    if(map.width <= 0)
        map.width = 1;

    if(map.height <= 0)
        map.height = 1;
}
}
