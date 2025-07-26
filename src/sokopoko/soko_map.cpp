#include "soko_map.hpp"
#include <tracy/Tracy.hpp>

namespace Sokoban {
bool load_map(Map& map, const std::string& file_path) {
    ZoneScopedN("Load map");
    yyjson_read_err err;
    yyjson_doc* doc = yyjson_read_file(file_path.c_str(), 0, NULL, &err);
    if(doc == NULL || err.pos != 0) {
        printf("JSON: %s at position %zu\n", err.msg, err.pos);
        return false;
    }

    yyjson_val* root = yyjson_doc_get_root(doc);

    for(int i = 0; i < 4; i++) {
        map.c_actors[i] = nullptr;
    }
    
    if(!map.tiles.empty()) {
        map.tiles.clear();
    }

    if(!map.objects.empty()) {
        for(SokoObject* object : map.objects) {
	    delete object;
        }
        map.objects.clear();
    }

    if(!map.props.empty()) {
        map.props.clear();
    }

    yyjson_val* name_val = yyjson_obj_get(root, "name");
    if(yyjson_is_str(name_val)) {
        map.name = yyjson_get_str(name_val);
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
	    if(yyjson_is_int(val)) {
                map.tiles.push_back(yyjson_get_int(val));
	    }
        }
    }

    yyjson_val* objects_arr = yyjson_obj_get(root, "objects");
    if(yyjson_is_arr(objects_arr)) {
        size_t idx, max;
        yyjson_val* val;
        yyjson_arr_foreach(objects_arr, idx, max, val) {
            yyjson_val* object_class = yyjson_obj_get(val, "class");
            yyjson_val* object_name  = yyjson_obj_get(val, "name");
            yyjson_val* object_x     = yyjson_obj_get(val, "x");
            yyjson_val* object_y     = yyjson_obj_get(val, "y");
	    
            // property flags, not required
            yyjson_val* object_hidden = yyjson_obj_get(val, "is_hidden");
            bool is_hidden = false;
            if(yyjson_is_bool(object_hidden)) { is_hidden = yyjson_get_bool(object_hidden); }

            yyjson_val* object_c_actor_pos = yyjson_obj_get(val, "c_actor_pos");
            int c_actor_pos = -1;
            if(yyjson_is_int(object_c_actor_pos)) { c_actor_pos = yyjson_get_int(object_c_actor_pos); }

            printf("class %i, name %s, x %i, y %i\n",
                   yyjson_get_int(object_class),
                   yyjson_get_str(object_name),
                   yyjson_get_int(object_x),
                   yyjson_get_int(object_y));
	   
            if(yyjson_is_int(object_class) && yyjson_is_int(object_x)
               && yyjson_is_int(object_y) && yyjson_is_str(object_name)) {
                SokoObject* obj = create_object(map,
                                                yyjson_get_str(object_name),
                                                static_cast<SokoObjectClass>(yyjson_get_int(object_class)),
                                                SokoPosition{yyjson_get_int(object_x), yyjson_get_int(object_y)});

                if(obj == nullptr) { continue; }

                // apply properties
                
                obj->hidden = is_hidden;
                if(c_actor_pos >= 0) {
                    map.c_actors[c_actor_pos] = obj;
                    printf("set object to c_actor pos %i\n", c_actor_pos);
                }
            }
        }
    }

    yyjson_val* cam_regions_arr = yyjson_obj_get(root, "cam_regions");
    if(yyjson_is_arr(cam_regions_arr)) {
        size_t idx, max;
        yyjson_val* val;
        yyjson_arr_foreach(objects_arr, idx, max, val) {
            yyjson_val* cam_region_x      = yyjson_obj_get(val, "x");
            yyjson_val* cam_region_y      = yyjson_obj_get(val, "y");
            yyjson_val* cam_region_width  = yyjson_obj_get(val, "width");
            yyjson_val* cam_region_height = yyjson_obj_get(val, "height");

            map.cam_regions.push_back({
                SokoRegion{
                    .x = yyjson_get_int(cam_region_x),
                    .y = yyjson_get_int(cam_region_y),
                    .width = yyjson_get_int(cam_region_width),
                    .height = yyjson_get_int(cam_region_height),
                },
                Vector2(120.0f, 40.0f)
                });
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

    if(!map.objects.empty()) {
        yyjson_mut_val* objects_key = yyjson_mut_str(doc, "objects");
        yyjson_mut_val* objects_arr = yyjson_mut_arr(doc);

        for(auto& obj : map.objects) {
            yyjson_mut_val* object_def = yyjson_mut_obj(doc);

            yyjson_mut_val* object_class_key = yyjson_mut_str(doc, "class");
            yyjson_mut_val* object_class_val = yyjson_mut_int(doc, obj->type);
            
            yyjson_mut_val* object_name_key = yyjson_mut_str(doc, "name");
            yyjson_mut_val* object_name_val = yyjson_mut_str(doc, obj->name.c_str());
            
            yyjson_mut_val* object_x_key = yyjson_mut_str(doc, "x");
            yyjson_mut_val* object_x_val = yyjson_mut_int(doc, obj->position.x);

            yyjson_mut_val* object_y_key = yyjson_mut_str(doc, "y");
            yyjson_mut_val* object_y_val = yyjson_mut_int(doc, obj->position.y);

            yyjson_mut_obj_add(object_def, object_class_key, object_class_val);
            yyjson_mut_obj_add(object_def, object_name_key, object_name_val);
            yyjson_mut_obj_add(object_def, object_x_key, object_x_val);
            yyjson_mut_obj_add(object_def, object_y_key, object_y_val);

            // properties
            if(obj->hidden) {
                yyjson_mut_val* object_hidden_key = yyjson_mut_str(doc, "is_hidden");
                yyjson_mut_val* object_hidden_val = yyjson_mut_bool(doc, obj->hidden);
                yyjson_mut_obj_add(object_def, object_hidden_key, object_hidden_val);
            }
            
            yyjson_mut_arr_append(objects_arr, object_def);
        }

        yyjson_mut_obj_add(root, objects_key, objects_arr);
    }

    yyjson_mut_doc_set_root(doc, root);
    bool suc = yyjson_mut_write_file(file_path.c_str(), doc, YYJSON_WRITE_PRETTY, NULL, NULL);
    yyjson_mut_doc_free(doc);

    return suc;
}

void draw_map(Map& map, Turbine::Batch& batch) {
    if (batch.texture == nullptr || batch.texture->width == 0) {
        return;
    }

    batch.begin();
    for(int i = 0; i < map.tiles.size(); i++) {
        uint16_t& tile_id = map.tiles[i];

        // Skip empty tiles.
        if(tile_id == -1) {
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

SokoObject* create_object(Map& map, const std::string& name, SokoObjectClass type, SokoPosition position) {
    SokoObject* obj = new SokoObject();
    
    switch(type) {
    case SokoObjectClass::BARRIER:
        obj->type = SokoObjectClass::BARRIER;
        obj->behaviour = SokoObjectBehaviour::STATIC;
        obj->hidden = true;
        obj->position = position;
        obj->sprite.offset.y = 6;
        break;
    case SokoObjectClass::WOODEN_CRATE:
        obj->type = SokoObjectClass::WOODEN_CRATE;
        obj->behaviour = SokoObjectBehaviour::CRATE;
        obj->position = position;
        obj->sprite.offset.y = 6;
        break;
    case SokoObjectClass::METAL_CRATE:
        obj->type = SokoObjectClass::METAL_CRATE;
        obj->behaviour = SokoObjectBehaviour::CRATE;
        obj->position = position;
        obj->sprite.offset.y = 6;
        break;
    case SokoObjectClass::PILLOW:
        obj->type = SokoObjectClass::PILLOW;
        obj->behaviour = SokoObjectBehaviour::CRATE;
        obj->position = position;
        obj->sprite.offset.y = 6;
        break;
    case SokoObjectClass::ROSS:
        obj->type = SokoObjectClass::ROSS;
        obj->behaviour = SokoObjectBehaviour::PLAYER;
        obj->position = position;
        break;
    case SokoObjectClass::MIRAGE:
        obj->type = SokoObjectClass::MIRAGE;
        obj->behaviour = SokoObjectBehaviour::NPC_FOLLOW;
        obj->position = position;
        break;
    }

    if(obj != nullptr) {
        obj->name = name;
        map.objects.push_back(obj);
    }

    return obj;
}

// Temporarily hardcoded. Switch to using serialized data.
void set_default_object_sprite(SokoObject* obj, Atlas& atlas) {
    if(obj == nullptr) { return; }
    
    switch(obj->type) {
    case SokoObjectClass::BARRIER:
        atlas.set_region(obj->sprite, "barrier");
        break;
    case SokoObjectClass::WOODEN_CRATE:
        atlas.set_region(obj->sprite, "crate_wood_s0");
        break;
    case SokoObjectClass::METAL_CRATE:
        atlas.set_region(obj->sprite, "crate_stone_s0");
        break;
    case SokoObjectClass::PILLOW:
        atlas.set_region(obj->sprite, "crate_pillow_s0");
        break;
    case SokoObjectClass::ROSS:
        atlas.set_region(obj->sprite, "ross_b_idle_s0");
        break;
    case SokoObjectClass::MIRAGE:
        atlas.set_region(obj->sprite, "mirage_idle_s0");
        break;
    }
}

SokoObject* object_at(Map& map, SokoPosition position) {
    for(int i = 0; i < map.objects.size(); i++) {
        if(map.objects[i]->position.x == position.x &&
           map.objects[i]->position.y == position.y) {
            return map.objects[i];
        }
    }
    return nullptr;
}

bool attempt_movement(Map& map, SokoObject* actor, SokoPosition destination) {
    if(actor == nullptr)
        return false;
    
    auto target = object_at(map, destination);
    
    if(target == nullptr) {
        actor->position = destination;
        return true;
    }

    if(target->behaviour == SokoObjectBehaviour::STATIC) {
        return false;
    }

    if(target->behaviour == SokoObjectBehaviour::GOAL ||
       target->behaviour == SokoObjectBehaviour::NPC_FOLLOW) {
        actor->position = destination;
        return true;
    }
    
    if(target->behaviour == SokoObjectBehaviour::DOOR) {
        // TODO: level loading / transitions
    }

    if(target->behaviour == SokoObjectBehaviour::CRATE) {
        SokoPosition movement_vector = {
            destination.x - actor->position.x,
            destination.y - actor->position.y,
        };

        SokoPosition box_target = {
            actor->position.x + (movement_vector.x * 2),
            actor->position.y + (movement_vector.y * 2)
        };

        auto obstacle = object_at(map, box_target);
        
        if(obstacle != nullptr) {
            if(obstacle->behaviour == SokoObjectBehaviour::GOAL) {
                actor->position = destination;
                target->position = box_target;
                return true;
            }
        } else {
            actor->position = destination;
            target->position = box_target;
            return true;
        }
    }
    
    return false;
}

bool check_puzzle_state(Map& map, uint32_t puzzle_id) {
    bool puzzle_is_solved = true;

    if(map.goals.empty())
        puzzle_is_solved = false;

    for(Goal& goal : map.goals) {
        // check for things in goals
        SokoObject* thing_in_goal = object_at(map, goal.position);
        if (thing_in_goal != nullptr && thing_in_goal->behaviour == SokoObjectBehaviour::CRATE) {
            goal.is_satisfied = true;
        }
        else {
            goal.is_satisfied = false;
        }

        // check all the goals
        if(goal.id == puzzle_id && goal.is_satisfied == false) {
            puzzle_is_solved = false;
        }
    }

    return puzzle_is_solved;
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
