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

void MenuState::move_player() {
    attempt_movement(map.objects, c_actor, {c_actor->position.x + dir_to_vec(c_actor->direction).x,
                                            c_actor->position.y + dir_to_vec(c_actor->direction).y});
}

void MenuState::init() {
    b1.initialize();
    b2.initialize();
    b3.initialize();
    
    Turbine::load_texture(map_texture,    "./res/sprite/map.png",     TB_NEAREST, TB_NEAREST, TB_DISCARD_RAW_TEXTURE_DATA);
    Turbine::load_texture(object_texture, "./res/sprite/atlas0.png",  TB_NEAREST, TB_NEAREST, TB_DISCARD_RAW_TEXTURE_DATA); // AUTOLOAD.
    Turbine::load_texture(texture,        "./res/sprite/ui.png",      TB_NEAREST, TB_NEAREST, TB_DISCARD_RAW_TEXTURE_DATA);
    b1.texture = &map_texture;
    b2.texture = &object_texture;
    b3.texture = &texture;

    Turbine::load_atlas(atlas, "./res/sprite/atlas.json");

    shadow_sprite.color = 0x7CFFFFFF;
    shadow_sprite.offset.y = 16;
    shadow_sprite.centered = true;
    atlas.set_region(shadow_sprite, "shadow_md");

    atlas.set_region(object_preview_sprite, "barrier");

    cam.origin.x   = 320.0f / 2.0f;
    cam.origin.y   = 240.0f / 2.0f;

    sky.initialize(&texture);
    load_map(map, "./res/level/mother.json");
    for (SokoObject* object : map.objects) { set_default_object_sprite(object, atlas); } // temporary
    c_actor = map.c_actors[0];
}

void MenuState::update(double delta_time, Turbine::InputState& input) {
    mouse_x = input.mouse_x / 4; // scale
    mouse_y = input.mouse_y / 4;

    cam.position = Vector2((map.width * 24.0f - 24.0f) / 2.0f, (map.height * 24.0f - 24.0f) / 2.0f);

    if(c_actor != nullptr) {
        if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_RIGHT)) {
            c_actor->direction = SokoDirection::EAST;
            c_actor->sprite.flip_h = false;
            atlas.set_region(c_actor->sprite, "ross_b_idle_e0");
            move_player();
        }
        if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_LEFT)) {
            c_actor->direction = SokoDirection::WEST;
            c_actor->sprite.flip_h = true;
            atlas.set_region(c_actor->sprite, "ross_b_idle_e0");
            move_player();
        }
        if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_UP)) {
            c_actor->direction = SokoDirection::NORTH;
            atlas.set_region(c_actor->sprite, "ross_lost_idle_n0");
            move_player();
        }
        if(Turbine::just_pressed(input, Turbine::InputAction::MOVE_DOWN)) {
            c_actor->direction = SokoDirection::SOUTH;
            atlas.set_region(c_actor->sprite, "ross_b_idle_s0");
            move_player();
        }
    }

    if (Turbine::just_pressed(input, Turbine::InputAction::EDITOR_PLACE_TILE)) {
        change_tile(map, {0, 2}, 3);
    }

    for(auto& object : map.objects) {
        object->sprite.position.x = std::lerp(object->sprite.position.x, object->position.x * 24, 35 * delta_time);
        object->sprite.position.y = std::lerp(object->sprite.position.y, (object->position.y * 24) - 12, 35 * delta_time);
    }
    
    // Update sprite list
    sprite_list.clear();
    sprite_list.reserve(map.objects.size() * 2);
    for (SokoObject* object : map.objects) {
        if(object->hidden) {
            if(map.show_hidden_objects) {
                object->sprite.color = 0x50FFFFFF;
                object->sprite.visible = true;
            } else {
                object->sprite.visible = false;
            }
        } else {
            object->sprite.color = 0xFFFFFFFF;
        }
        
        sprite_list.push_back(&object->sprite);
    }

    std::sort(sprite_list.begin(), sprite_list.end(), [](const Sprite* a, const Sprite* b) {
        return a->position.y < b->position.y;
    });
    
    cam.position.x = floor(cam.position.x);
    cam.position.y = floor(cam.position.y);
}

void MenuState::draw(Turbine::Window& window, Turbine::Shader& base_shader) {
    Turbine::clear(window, 0.0f, 0.0f, 0.0f);
    Turbine::set_blend_mode(window);

    Turbine::reset_camera(cam, base_shader);
    sky.render();

    Turbine::apply_camera(cam, base_shader);
    draw_map(map, b1);

    b2.begin();
    for (SokoObject* object : map.objects) {
        if(object->hidden) {
            if(map.show_hidden_objects) {
                shadow_sprite.visible = true;
            } else {
                shadow_sprite.visible = false;
            }
        } else {
            shadow_sprite.visible = true;
        }
        
        shadow_sprite.position = object->sprite.position;
        b2.queue(shadow_sprite);
    }
    for(Turbine::Sprite* sprite : sprite_list) {        
        b2.queue(*sprite);
    }
    // b2.queue(object_preview_sprite);
    b2.end();

    Turbine::reset_camera(cam, base_shader);
    Sokoban::imgui_map_inspect(map, c_actor, sky, atlas);
    Sokoban::imgui_tile_inspect(map, map_texture);
}
}
