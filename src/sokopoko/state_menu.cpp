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
    b1.initialize();
    b2.initialize();
    b3.initialize();
    
    Turbine::load_texture(map_texture,    "./res/map.png",     TB_NEAREST, TB_NEAREST, false);
    Turbine::load_texture(object_texture, "./res/objects.png", TB_NEAREST, TB_NEAREST, false);
    Turbine::load_texture(texture,        "./res/ui.png",      TB_NEAREST, TB_NEAREST, false);
    b1.texture = &map_texture;
    b2.texture = &object_texture;
    b3.texture = &texture;

    shadow_sprite.color = 0x7CFFFFFF;
    shadow_sprite.offset.y = 16;
    shadow_sprite.centered = true;
    shadow_sprite.region = Turbine::Rect{0, 113, 24, 12};

    cam.origin.x   = 400.0f / 2.0f;
    cam.origin.y   = 240.0f / 2.0f;

    load_map(map, "/home/robbi/Desktop/test_map.json");
    
    // temporary hardcode
    anim.name = "walk_s";
    int pt = Turbine::add_track(anim, TrackType::POSITION);
    int rt = Turbine::add_track(anim, TrackType::REGION);

    Turbine::set_track_target(anim, pt, "sprite.offset");

    Turbine::set_track_target(anim, rt, "sprite.region");
    Turbine::region_track_insert_key(anim, rt, 0.0 , Turbine::Rect{96, 0, 32, 42});
    Turbine::region_track_insert_key(anim, rt, 0.25, Turbine::Rect{128, 0, 32, 42});
    Turbine::region_track_insert_key(anim, rt, 0.5 , Turbine::Rect{0,  0, 32, 42});
}

void MenuState::update(double delta_time, Turbine::InputState& input) {
    mouse_x = input.mouse_x / 3; // scale
    mouse_y = input.mouse_y / 3;

    cam.position.x = floor(((map.width  * 24.0f) - 12) / 2.0f);
    cam.position.y = floor(((map.height * 24.0f) - 12) / 2.0f);

    current_time += delta_time * 2;

    if(c_actor != nullptr) {
        SokoObject* actor = c_actor;
        if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_UP)) {
            attempt_movement(map.objects, actor, SokoPosition{actor->position.x, actor->position.y - 1});
            current_time = 0.0f;
        }
        if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_DOWN)) {
            attempt_movement(map.objects, actor, SokoPosition{actor->position.x, actor->position.y + 1});
            current_time = 0.0f;
        }
        if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_LEFT)) {
            attempt_movement(map.objects, actor, SokoPosition{actor->position.x - 1, actor->position.y});
            actor->sprite.flip_h = true;
            current_time = 0.0f;
        }
        if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_RIGHT)) {
            attempt_movement(map.objects, actor, SokoPosition{actor->position.x + 1, actor->position.y});
            actor->sprite.flip_h = false;
            current_time = 0.0f;
        }
    }

    for(auto& object : map.objects) {
        object->sprite.position.x = object->position.x * 24;
        object->sprite.position.y = (object->position.y * 24) - 12;
    }

    RegionTrack* rtt = static_cast<RegionTrack*>(Turbine::get_track(anim, 1));
    if(!rtt->regions.empty()) {
        const TKey<Turbine::Rect>& a = Turbine::get_lower_bound_key<Turbine::Rect>(rtt->regions, current_time);
    	if(c_actor != nullptr) {
	        c_actor->sprite.region = a.value;
	    }
    }

    PositionTrack* ptt = static_cast<PositionTrack*>(Turbine::get_track(anim, 0));
    if(!ptt->positions.empty()) {
        const TKey<Turbine::Vector2>& b = Turbine::get_lower_bound_key<Turbine::Vector2>(ptt->positions, current_time);
	    if(c_actor != nullptr) {
            c_actor->sprite.offset = b.value;
        }
    }
}

void MenuState::draw(Turbine::Window& window, Turbine::Shader& base_shader) {
    Turbine::clear(window, 0.0f, 0.0f, 0.0f);
    Turbine::set_blend_mode(window);

    Turbine::apply_camera(cam, base_shader);
    draw_map(map, b1);

    b2.begin();
    for(SokoObject* object : map.objects) {
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
    Turbine::queue_bitmap_string(b3, "Hahaha, that's funny.\nUhh.. maybe we should talk about that later.", 0, 0);
    b3.end();

    Turbine::imgui_draw_timeline(anim);
    Sokoban::imgui_map_inspect(map, c_actor);
}
}
