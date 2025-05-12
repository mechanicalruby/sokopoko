#include "state_menu.hpp"

namespace Sokoban {
MenuState::~MenuState() {
    for(int i = 0; i < map.objects.size(); i++) {
        delete map.objects[i];
    }
}

SokoObject* MenuState::find_player(const ObjectList& list) {
    for(auto obj : list) {
        if(obj->behaviour == SokoObjectBehaviour::PLAYER) {
            return obj;
        }
    }
    return nullptr;
}

void MenuState::init() {
    // Loading
    Turbine::load_texture(texture,        "./res/ui.png", TB_NEAREST, TB_NEAREST, false);
    Turbine::load_texture(object_texture, "./res/objects.png", TB_NEAREST, TB_NEAREST, false);
    Turbine::load_texture(map_texture,    "./res/map.png", TB_NEAREST, TB_NEAREST, false);
    b1.texture = &map_texture;
    b2.texture = &object_texture;
    b3.texture = &texture;

    shadow_sprite.color = 0x7CFFFFFF;
    shadow_sprite.offset.y = 16;
    shadow_sprite.centered = true;
    shadow_sprite.region = Turbine::Rect{0, 113, 24, 12};

    load_map(map, "C:/Users/robbi/Desktop/mother.json");
    
    cam.origin.x   = 400.0f / 2.0f;
    cam.origin.y   = 240.0f / 2.0f;

    create_object(map, SokoObjectClass::ROSS, SokoPosition{2, 2});
    c_actor = find_player(map.objects);

    PositionTrack* position_track = static_cast<PositionTrack*>(anim.add_track(TrackType::TYPE_POSITION));
    RegionTrack*   region_track   = static_cast<RegionTrack*>(anim.add_track(TrackType::TYPE_REGION));

    position_track->keys.push_back(TKey<Vector2>{0.0, Vector2{ 0.0f, -2.0f}});
    position_track->keys.push_back(TKey<Vector2>{2.0, Vector2{-2.0f,  0.0f}});
    position_track->keys.push_back(TKey<Vector2>{2.0, Vector2{ 0.0f,  2.0f}});
    position_track->keys.push_back(TKey<Vector2>{2.0, Vector2{ 2.0f,  0.0f}});
}

void MenuState::update(double delta_time, Turbine::InputState& input) {
    mouse_x = input.mouse_x / 3; // scale
    mouse_y = input.mouse_y / 3;

    cam.position.x = ((map.width  * 24.0f) - 12) / 2.0f;
    cam.position.y = ((map.height * 24.0f) - 12) / 2.0f;

    if(c_actor != nullptr && c_actor->type == SokoObjectClass::ROSS) {
        Ross* actor = static_cast<Ross*>(c_actor);
        
        if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_UP)) {
            attempt_movement(map.objects, actor, SokoPosition{actor->position.x, actor->position.y - 1});
            
            actor->step.play();
            actor->sprite.region = Rect{32 * 2, 0, 32, 42};
        }
        if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_DOWN)) {
            attempt_movement(map.objects, actor, SokoPosition{actor->position.x, actor->position.y + 1});
            
            actor->step.play();
            actor->sprite.region = Rect{0, 0, 32, 42};
        }
        if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_LEFT)) {
            attempt_movement(map.objects, actor, SokoPosition{actor->position.x - 1, actor->position.y});
            
            actor->step.play();
            actor->sprite.region = Rect{32, 0, 32, 42};
            actor->sprite.flip_h = true;
        }
        if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_RIGHT)) {
            attempt_movement(map.objects, actor, SokoPosition{actor->position.x + 1, actor->position.y});
            
            actor->step.play();
            actor->sprite.region = Rect{32, 0, 32, 42};
            actor->sprite.flip_h = false;
        }
    }

    if(map.is_edit_mode) {
        if(Turbine::just_pressed(input, Turbine::InputAction::EDITOR_PLACE_TILE)) {
            change_tile(map, SokoPosition{static_cast<int>((mouse_x+12) / 24), static_cast<int>((mouse_y+12) / 24)}, 2);
        }
    }

    for(auto& object : map.objects) {
        object->sprite.position.x = static_cast<float>(object->position.x * 24);
        object->sprite.position.y = static_cast<float>((object->position.y * 24) - 12);
    }
}

void MenuState::draw(Turbine::Window& window, Turbine::Shader& base_shader) {
    Turbine::clear(window, 0.0f, 0.0f, 0.0f);
    Turbine::set_blend_mode(window);

    Turbine::apply_camera(cam, base_shader);
    draw_map(map, b1);

    b2.begin();
    for(auto& object : map.objects) {
        if(object->hidden) {
            if(map.show_hidden_objects) {
                object->sprite.color = 0x50FFFFFF;
                object->sprite.visible = true;
                shadow_sprite.visible = true;
            } else {
                object->sprite.visible = false;
                shadow_sprite.visible = false;
            }
        } else {
            shadow_sprite.visible = true;
            object->sprite.color = 0xFFFFFFFF;
        }
        
        shadow_sprite.position = object->sprite.position;
        
        b2.queue(shadow_sprite);
        b2.queue(object->sprite);
    }
    b2.end();
    
    Turbine::reset_camera(cam, base_shader);

    b3.begin();
    // Turbine::queue_bitmap_string(b3, "Sokopoko!", 12, 12);
    b3.end();

    Turbine::imgui_draw_timeline(anim);
    Sokoban::imgui_map_inspect(map);    
}
}
