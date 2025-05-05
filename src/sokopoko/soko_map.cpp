#include "soko_map.hpp"

namespace Sokoban {
bool load_map(Map& map, const std::string& file_path) {
    yyjson_read_err err;
    yyjson_doc* doc = yyjson_read_file(file_path.c_str(), 0, NULL, &err);
    if(doc == NULL || err.pos != 0) {
        printf("JSON: %s at position %zu", err.msg, err.pos);
        return false;
    }

    yyjson_val* root = yyjson_doc_get_root(doc);

    if(!map.tiles.empty()) {
        map.tiles.clear();
    }
    
    yyjson_val* width_val = yyjson_obj_get(root, "width");
    if(yyjson_is_int(width_val)) {
        map.width = yyjson_get_int(width_val);
    }

    yyjson_val* height_val = yyjson_obj_get(root, "height");
    if(yyjson_is_int(height_val)) {
        map.height = yyjson_get_int(height_val);
    }
    
    yyjson_val* tiles_arr = yyjson_obj_get(root, "tiles");
    if(yyjson_is_arr(tiles_arr)) {
        size_t idx, max;
        yyjson_val* val;
        yyjson_arr_foreach(tiles_arr, idx, max, val) {
            map.tiles.push_back(yyjson_get_int(val));
        }
    }

    yyjson_val* objects_arr = yyjson_obj_get(root, "objects");
    if(yyjson_is_arr(objects_arr)) {
        size_t idx, max;
        yyjson_val* val;
        yyjson_arr_foreach(objects_arr, idx, max, val) {
            const char* type = yyjson_get_str(val);
            
            switch(type) {
            case "player":
                map.objects.push_back(new Player{0, 0});
                break;
            default:
                break;
            }
        }
    }
    
    yyjson_doc_free(doc);
    return true;
}

bool save_map(Map& map, const std::string& file_path) {
    if(file_path.empty())
        return false;
    
    yyjson_mut_doc* doc = yyjson_mut_doc_new(NULL);
    yyjson_mut_val* root = yyjson_mut_obj(doc);

    if(!map.name.empty()) {
        yyjson_mut_val* name_key = yyjson_mut_str(doc, "name");
        yyjson_mut_val* name_val = yyjson_mut_str(doc, map.name.c_str());
        yyjson_mut_obj_add(root, name_key, name_val);
    }

    if(map.width != 0 && map.height != 0) {
        yyjson_mut_val* width_key = yyjson_mut_str(doc, "width");
        yyjson_mut_val* width_val = yyjson_mut_int(doc, map.width);

        yyjson_mut_val* height_key = yyjson_mut_str(doc, "height");
        yyjson_mut_val* height_val = yyjson_mut_int(doc, map.height);

        yyjson_mut_obj_add(root, width_key, width_val);
        yyjson_mut_obj_add(root, height_key, height_val);
    }

    if(!map.tiles.empty()) {
        yyjson_mut_val* tiles_key = yyjson_mut_str(doc, "tiles");
        yyjson_mut_val* tiles_arr = yyjson_mut_arr(doc);
        
        for(auto& tile : map.tiles) {
            yyjson_mut_val* id = yyjson_mut_int(doc, tile);
            yyjson_mut_arr_append(tiles_arr, id);
        }

        yyjson_mut_obj_add(root, tiles_key, tiles_arr);
    }

    yyjson_mut_doc_set_root(doc, root);
    bool suc = yyjson_mut_write_file(file_path.c_str(), doc, YYJSON_WRITE_PRETTY, NULL, NULL);
    yyjson_mut_doc_free(doc);

    return suc;
}

void draw_map(Map& map, Turbine::Batch& batch) {
    batch.begin();
    for(int i = 0; i < map.tiles.size(); i++) {
        uint16_t& tile_id = map.tiles[i];

        // Skip empty tiles.
        if(tile_id == 2) {
            continue;
        }

        int texture_width = batch.texture->width;
        int column_width = texture_width / 24; // nearest whole multiple 

        int tile_x = tile_id % column_width;
        int tile_y = tile_id / column_width;

        map.tile_sprite.region   = Turbine::Rect(tile_x * 24.0f, tile_y * 24.0f, 24.0f, 24.0f);
        map.tile_sprite.position = Turbine::Vector2((i % map.width) * 24.0f,
                                                    (i / map.width) * 24.0f);

        batch.queue(map.tile_sprite);
    }
    batch.end();
}

void change_tile(Map& map, SokoPosition position, uint16_t new_id) {
    if((position.x < 0 || position.x >= map.width) ||
       (position.y < 0 || position.y >= map.height))
        return;

    if((position.x + position.y * map.width) < map.tiles.size())
        map.tiles[position.x + position.y * map.width] = new_id;
}

std::string save_file_prompt() {
    NFD::Guard nfdGuard;
    NFD::UniquePath outPath;

    nfdfilteritem_t filterItem[1] = {{"Sokopoko! map", "json"}};
    nfdresult_t result = NFD::SaveDialog(outPath, filterItem, 1);
    if (result == NFD_OKAY) {
        return outPath.get();
    } else if (result == NFD_CANCEL) {
        return {};
    } else {
        std::cout << "Error: " << NFD::GetError() << std::endl;
    }

    return {};
}

std::string load_file_prompt() {
    NFD::Guard nfdGuard;
    NFD::UniquePath outPath;

    nfdfilteritem_t filterItem[1] = {{"Sokopoko! map", "json"}};
    nfdresult_t result = NFD::OpenDialog(outPath, filterItem, 1);
    if (result == NFD_OKAY) {
        return outPath.get();
    } else if (result == NFD_CANCEL) {
        return {};
    } else {
        std::cout << "Error: " << NFD::GetError() << std::endl;
    }

    return {};
}
}
