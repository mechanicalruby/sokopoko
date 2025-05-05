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
    
    ImGui::Checkbox("Show hidden objects", &map.show_hidden_objects);

    ImGui::Separator(); // Objects

    for(int i = 0; i < map.objects.size(); i++) {
        SokoObject* object = map.objects[i];

        if(object->hidden && !map.show_hidden_objects)
            continue;

        ImGui::PushID(object);
        if (ImGui::TreeNode(object->name.c_str())) {
            ImGui::Text("Position: %i, %i", object->position.x, object->position.y);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    
    ImGui::End();

    if(map.width <= 0)
        map.width = 1;

    if(map.height <= 0)
        map.height = 1;
}
}
