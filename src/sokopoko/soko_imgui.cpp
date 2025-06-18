#include "soko_imgui.hpp"

namespace Sokoban {
void imgui_map_inspect(Map& map, SokoObject*& c_actor, Sky& sky) {
    ImGui::Begin("Map");

    if(ImGui::Button("Load map")) {
        if(load_map(map, load_file_prompt())) {
            c_actor = map.c_actors[0];
        }
    }
    ImGui::SameLine();
    if(ImGui::Button("Save map")) {
        save_map(map, save_file_prompt());
    }

    ImGui::InputInt("Width", &map.width);
    ImGui::InputInt("Height", &map.height);

    ImGui::Separator(); // Settings

    ImGui::Checkbox("Edit mode", &map.is_edit_mode);
    ImGui::SameLine();
    ImGui::Checkbox("Sky tint", &map.is_edit_mode); // add setting
    ImGui::Checkbox("Show hidden objects", &map.show_hidden_objects);

    ImGui::Separator(); // Objects
    ImGui::Text("Sky");

    float top_val[3] = {
        sky.top_color.rgba.r / 255.0f,
        sky.top_color.rgba.g / 255.0f,
        sky.top_color.rgba.b / 255.0f
    };
    float bottom_val[3] = {
        sky.bottom_color.rgba.r / 255.0f,
        sky.bottom_color.rgba.g / 255.0f,
        sky.bottom_color.rgba.b / 255.0f
    };
    
    if(ImGui::ColorEdit3("Top", top_val)) {
        sky.top_color.rgba.r = static_cast<unsigned char>(top_val[0] * 255.0f);
        sky.top_color.rgba.g = static_cast<unsigned char>(top_val[1] * 255.0f);
        sky.top_color.rgba.b = static_cast<unsigned char>(top_val[2] * 255.0f);
    }

    if(ImGui::ColorEdit3("Bottom", bottom_val)) {
        sky.bottom_color.rgba.r = static_cast<unsigned char>(bottom_val[0] * 255.0f);
        sky.bottom_color.rgba.g = static_cast<unsigned char>(bottom_val[1] * 255.0f);
        sky.bottom_color.rgba.b = static_cast<unsigned char>(bottom_val[2] * 255.0f);
    }
    
    ImGui::Separator(); // Objects
    ImGui::Text("Objects");
    ImGui::SameLine();
    if(c_actor != nullptr) {
        ImGui::Text("c_actor: %s", c_actor->name.c_str());
    } else {
        ImGui::Text("c_actor is NULL", c_actor);
    }

    static int selection = 0;
    ImGui::PushItemWidth(-1);
    if (ImGui::BeginListBox("##objectlist")) {
        for (int i = 0; i < map.objects.size(); i++) {
            SokoObject* object = map.objects[i];
            if (object == nullptr) {
                ImGui::EndListBox();
                return;
            }
            if (object->hidden && !map.show_hidden_objects)
                continue;

            ImGui::PushID(object); // Use object pointer as unique ID

            const bool is_selected = (selection == i);
            if (ImGui::Selectable(object->name.c_str(), is_selected)) {
                selection = i;
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }

            ImGui::PopID();
        }
        ImGui::EndListBox();
    }
    ImGui::PopItemWidth();

    ImGui::Separator(); // Inspector
    ImGui::Text("Inspector");

    {
        SokoObject* object = nullptr;
        try {
            object = map.objects.at(selection);
        } catch (const std::out_of_range& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }

        if(object != nullptr) { 
            if(ImGui::Button("Set current actor")) {
                c_actor = object;
            }
            ImGui::SameLine();
            if(ImGui::Button("Delete object")) {
                c_actor = object;
            }
        
            ImGui::DragInt("x", &object->position.x, 0.25f);
            ImGui::DragInt("y", &object->position.y, 0.25f);
            ImGui::Checkbox("Hidden", &object->hidden);

            // Behaviour dropdown
            const char* behaviour_labels[] = {
                "STATIC", "CRATE", "DOOR", "GOAL", "PLAYER", "NPC_STATIC", "NPC_FOLLOW"
            };
            int behaviour_index = static_cast<int>(object->behaviour);
            if (ImGui::Combo("Behaviour", &behaviour_index, behaviour_labels, IM_ARRAYSIZE(behaviour_labels))) {
                object->behaviour = static_cast<SokoObjectBehaviour>(behaviour_index);
            }

            // Direction dropdown
            const char* direction_labels[] = {
                "NORTH", "EAST", "SOUTH", "WEST"
            };
            int direction_index = static_cast<int>(object->direction);
            if (ImGui::Combo("Direction", &direction_index, direction_labels, IM_ARRAYSIZE(direction_labels))) {
                object->direction = static_cast<SokoDirection>(direction_index);
            }

            if(object->behaviour == SokoObjectBehaviour::NPC_FOLLOW) {
                int party_pos = static_cast<int>(object->party_position);
                if (ImGui::InputInt("Party pos", &party_pos)) {
                    party_pos = std::clamp(party_pos, 0, 3);
                    object->party_position = static_cast<unsigned int>(party_pos);
                }
                
                if(object->party_position < 0)
                    object->party_position = 0;
            }
        } else {
            ImGui::Text("Select a SokoObject.");
        }
    }

    ImGui::End();

    ImGui::Begin("Palette");
    
    ImGui::End();
    
    if(map.width <= 0)
        map.width = 1;

    if(map.height <= 0)
        map.height = 1;
}
}
