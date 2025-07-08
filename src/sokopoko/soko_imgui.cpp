#include "soko_imgui.hpp"

namespace Sokoban {
void imgui_map_inspect(Map& map, SokoObject*& c_actor, Sky& sky, Turbine::Atlas& atlas) {
    ImGui::Begin("Map");

    if(ImGui::Button("Load map")) {
        if(load_map(map, load_file_prompt())) {
            c_actor = map.c_actors[0];
            for (SokoObject* object : map.objects) { set_default_object_sprite(object, atlas); } // temporary
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

    static int selection = 0;
    if(selection < 0 || selection > map.objects.size()) {
        selection = 0;
    }
    
    ImGui::PushItemWidth(-1);
    if (ImGui::BeginListBox("##objectlist")) {
        for (int i = 0; i < map.objects.size(); i++) {
            SokoObject* object = map.objects[i];
            
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
            // std::cout << "Exception: " << e.what() << std::endl;
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

            // Movement state info
            static const char* state_labels[] = {
                "IDLE", "IN_TRANSIT"
            };
            ImGui::Text("move state: %s", state_labels[object->state]);
            ImGui::Text("move progr: %f", object->move_progress);

            // Behaviour dropdown
            static const char* behaviour_labels[] = {
                "STATIC", "CRATE", "DOOR", "GOAL", "PLAYER", "NPC_STATIC", "NPC_FOLLOW"
            };
            int behaviour_index = static_cast<int>(object->behaviour);
            if (ImGui::Combo("Behaviour", &behaviour_index, behaviour_labels, IM_ARRAYSIZE(behaviour_labels))) {
                object->behaviour = static_cast<SokoObjectBehaviour>(behaviour_index);
            }

            // Direction dropdown
            static const char* direction_labels[] = {
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
    
    if(map.width <= 0)
        map.width = 1;

    if(map.height <= 0)
        map.height = 1;
}

void imgui_tile_inspect(Map& map, Texture& texture) {
    constexpr int TILE_SIZE = 24;

    ImGui::Begin("Tiles");

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos(); // top-left corner
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    ImVec2 canvas_limit = ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y);

    draw_list->AddRectFilled(canvas_pos, canvas_limit, IM_COL32(30, 30, 30, 255));
    draw_list->AddRect(canvas_pos, canvas_limit, IM_COL32(17, 17, 17, 255));
    draw_list->PushClipRect(canvas_pos, canvas_limit, true);

    // calculate number of usable tiles
    int usable_columns = texture.width / TILE_SIZE;
    int usable_rows    = texture.height / TILE_SIZE;

    int used_tex_width  = usable_columns * TILE_SIZE;
    int used_tex_height = usable_rows * TILE_SIZE;

    // snap UVs to usable tile area
    ImVec2 uv0 = ImVec2(0.0f, 0.0f);
    ImVec2 uv1 = ImVec2(
        static_cast<float>(used_tex_width)  / texture.width,
        static_cast<float>(used_tex_height) / texture.height
    );

    ImVec2 image_end = ImVec2((float)used_tex_width + canvas_pos.x, (float)used_tex_height + canvas_pos.y);

    draw_list->AddImage(
        texture.id,
        canvas_pos,
        image_end,
        uv0,
        uv1,
        IM_COL32(255, 255, 255, 255)
    );

    // image borders
    draw_list->AddRect(canvas_pos, image_end, IM_COL32(255, 0, 0, 120));

    // tile highlight
    ImVec2 mouse_pos = ImGui::GetIO().MousePos;
    if (mouse_pos.x >= canvas_pos.x && mouse_pos.y >= canvas_pos.y &&
        mouse_pos.x < image_end.x  && mouse_pos.y < image_end.y) {

        ImVec2 local = ImVec2(mouse_pos.x - canvas_pos.x, mouse_pos.y - canvas_pos.y);
        int tile_x = (int)(local.x) / TILE_SIZE;
        int tile_y = (int)(local.y) / TILE_SIZE;

        // clamp to usable tile area
        if (tile_x < usable_columns && tile_y < usable_rows) {
            ImVec2 tile_min = ImVec2(tile_x * TILE_SIZE + canvas_pos.x, tile_y * TILE_SIZE + canvas_pos.y);
            ImVec2 tile_max = ImVec2(TILE_SIZE + tile_min.x, TILE_SIZE + tile_min.y);
            draw_list->AddRect(tile_min, tile_max, IM_COL32(255, 255, 0, 200), 0.0f, 0, 1.0f);
        }
    }

    draw_list->PopClipRect();
    ImGui::End();
}
}
